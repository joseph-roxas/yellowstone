
//Defines inline code so the device can do a self reset ------------------------------------
// NOTE: this part is Arduino Due specific
#define SYSRESETREQ    (1<<2)
#define VECTKEY        (0x05fa0000UL)
#define VECTKEY_MASK   (0x0000ffffUL)
#define AIRCR          (*(uint32_t*)0xe000ed0cUL) // fixed arch-defined address
#define REQUEST_EXTERNAL_RESET (AIRCR=(AIRCR&VECTKEY_MASK)|VECTKEY|SYSRESETREQ);


#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192, 168, 0, 2);  // numeric IP for Antamedia Hotspot (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 3);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
int const BUFFER_SIZE =7;
char response[BUFFER_SIZE];

int count = 0;
int i = 0;




const byte interruptPin0 = 2;
const byte interruptPin1 = 3;
volatile unsigned int IRQcount = 0;
volatile unsigned long pulseTime;
char cstr[2];
char pass[7] = "DZ1LU";
volatile unsigned long fallTime = 0;
volatile unsigned long riseTime = 0;
volatile unsigned long lowTime = 0;
volatile unsigned long highTime = 0;
int buttonState;


String timestring;


byte L0[] = {0x1b,0x40,0x1b,0x61,0x01};
byte L1[] = {0x54,0x68,0x61,0x6e,0x6b,0x20,0x79,0x6f,0x75,0x20,0x66,0x6f,0x72,0x20, // Thank you for_
             0x63,0x68,0x6f,0x6f,0x73,0x69,0x6e,0x67,0x20, // choosing_
             0x41,0x49,0x52,0x20,0x46,0x49,0x42,0x52,0x45}; // AIR FIBRE
byte L2[] = {0x0d,0x0a,0x0a};
byte L3[] = {0x50,0x6c,0x65,0x61,0x73,0x65,0x20,0x6c,0x6f,0x67,0x69,0x6e}; // Please Login
byte L4[] = {0x0d,0x0a,0x0a};
byte L5[] = {0x1b,0x40,0x1b,0x61,0x01,0x1d,0x21,0x11};
byte L6[] = {0x4a,0x55,0x53,0x45,0x50}; // ACCOUNT CODE
byte L7[] = {0x0d,0x0a};
byte L8[] = {0x1d,0x21,0x10,0x0a};
byte L9[] = {0x1b,0x40,0x1b,0x61,0x01};
byte L10[] = {0x54,0x69,0x6d,0x65,0x3a,0x20}; // Time:_
byte L11[] = {0x32}; // Time Variable
byte L12[] = {0x20,0x68,0x6f,0x75,0x72,0x2f,0x73}; // _hour/s
byte L13[] = {0x0d,0x0a};
byte L14[] = {0x50,0x6c,0x65,0x61,0x73,0x65,0x20,0x6b,0x65,0x65,0x70,0x20, // Please_
              0x79,0x6f,0x75,0x72,0x20,0x54,0x69,0x63,0x6b,0x65,0x74}; // keep your Ticket
byte L15[] = {0x0d,0x0a};
byte L16[] = {0x0a,0x0a,0x0a,0x0a,0x0a,0x0a};
byte L17[] = {0x1b,0x69};

const int ledPin = 13;

int k;

void setup() {
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
  //Serial.println("connecting...");

  
  pinMode(interruptPin0, INPUT_PULLUP);
  pinMode(interruptPin1, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin0), detectFallTime, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), detectRiseTime, RISING);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  if (IRQcount > 0 && millis() - fallTime > 4500){
    startPrintSequence();
  }  
REQUEST_EXTERNAL_RESET; //reset watchdog
delay(10000);
}

void detectFallTime() {
  fallTime = millis();
}

void detectRiseTime() {
  riseTime = millis();
  lowTime = riseTime - fallTime;
  if (lowTime > 27 && lowTime < 34){
   IRQcount ++;
  }
}


void printThermal() {
  Serial.write(L0, sizeof(L0));
  Serial.write(L1, sizeof(L1));
  Serial.write(L2, sizeof(L2));
  Serial.write(L3, sizeof(L3));
  Serial.write(L4, sizeof(L4));
  Serial.write(L5, sizeof(L5));
  Serial.write(response);
  Serial.write(L7, sizeof(L7));
  Serial.write(L8, sizeof(L8));
  Serial.write(L9, sizeof(L9));
  Serial.write(L10, sizeof(L10));
  Serial.write(itoa(IRQcount*3, cstr, 10));
  Serial.write(L12, sizeof(L12));
  Serial.write(L13, sizeof(L13));
  Serial.write(L14, sizeof(L14));
  Serial.write(L15, sizeof(L15));
  Serial.write(L16, sizeof(L16));
  Serial.write(L17, sizeof(L17));

}


void getAccountName() {
  
  count = 0;
  i = 0;
  response[0] = (char)0;
 
  while (client.connect(server, 82)) {
    //Serial.println("connected");
  }
 
  if (client.connected()) {
    // Make an API request:
    client.print("GET /generateaccounts?number=1&priceplan=7&pass=1JC8QBAQ037FNQL37JSBBSLP83EC60&type=2&sell=1&passlength=5&inactivity=0&timeleft=");
    client.print(timestring); //timestring
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.0.2:82");
    client.println("Connection: close");
    client.println();
  }
  
  while (true){
    while (client.available()) {
      char c = client.read();
      //Serial.print(c);
      //Serial.print(count);
      count++;
      if (count >= 1476 && count < 1481){
        response[i] = c;
        i++;
      }
    }
    if (!client.connected()) {
      response[BUFFER_SIZE-1] = '\0';
      //Serial.println(count);
      //Serial.println((char*)response);
      //Serial.println();
      //Serial.println("disconnecting.");
      
      client.stop();
      break;
    }
  }
}

void startPrintSequence() {
      digitalWrite(ledPin, HIGH);
      REQUEST_EXTERNAL_RESET; //reset watchdog
      // Test Watchdog Timer
      delay(10000);
      
      timestring = ApiTimeLeft();
      getAccountName();
      printThermal();
      IRQcount = 0;
      digitalWrite(ledPin, LOW);
}


String ApiTimeLeft() {
    switch (IRQcount) {
    case 2:
      return String(21600);
      break;
    case 4:
      return String(43200);
      break;
    case 6:
      return String(64800);
      break;
    case 8:
      return String(86400); // 1 day
      break;
    case 10:
      return String(108000);
      break;
    case 12:
      return String(129600);
      break;
    case 14:
      return String(151200);
      break;
    case 16:
      return String(172800); // 2 days
      break;
    case 18:
      return String(194400);
      break;
    case 20:
      return String(216000);
      break;
    case 22:
      return String(237600);
      break;
    case 24:
      return String(259200); // 3 days
      break;
    case 26:
      return String(280800);
      break;
    case 28:
      return String(302400);
      break;
    case 30:
      return String(324000);
      break;
    case 32:
      return String(345600); // 4 days
      break;
    case 34:
      return String(367200);
      break;
    case 36:
      return String(388800);
      break;
    case 38:
      return String(410400);
      break;
    case 40:
      return String(432000); // 5 days
      break;
    case 42:
      return String(453600);
      break;
    case 44:
      return String(475200);
      break;
    case 46:
      return String(496800);
      break;
    case 48:
      return String(518400); // 6 days
    case 50:
      return String(540000);
      break;
    case 52:
      return String(561600);
      break;
    case 54:
      return String(583200);
      break;
    case 56:
      return String(604800); // 7 days
      break;
    case 58:
      return String(626400);
      break;
    case 60:
      return String(648000);
      break;
    case 62:
      return String(669600);
      break;
    case 64:
      return String(691200); // 8 days
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }
}


