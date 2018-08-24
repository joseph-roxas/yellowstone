
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




const byte interruptPin = 2;
volatile int IRQcount = 0;
volatile int LOWcount = 0;
volatile int HIGHcount =0;
volatile int newInsertedCoin = 0;
volatile int coinInserted = 0;
volatile unsigned long pulseTime;
char cstr[2];
char pass[7] = "DZ1LU";


String timestring;
volatile int timeleft = 0;

byte L1[] = {0x0a};
byte L2[] = {0x1b,0x40,0x1b,0x61,0x01,0x1d,0x21,0x10,0x57,0x49,0x46,0x49,0x56,0x45,0x0d,0x0a};
byte L3[] = {0x0a};
byte L4[] = {0x1b,0x40,0x1b,0x61,0x01,0x50,0x6c,0x65,0x61,0x73,0x65,0x20,0x6c,0x6f,0x67,0x69,0x6e,0x0d,0x0a};
byte L5[] = {0x0a};
byte L6s[] = {0x1b,0x40,0x1b,0x61,0x01,0x1d,0x21,0x11};
byte L6e[] = {0x0d,0x0a};
byte L7[] = {0x1d,0x21,0x10,0x0a};
byte L8s[] = {0x1b,0x40,0x1b,0x61,0x01,0x54,0x69,0x6d,0x65,0x3a,0x20};
byte L8e[] = {0x20,0x48,0x6f,0x75,0x72,0x2f,0x73,0x0d,0x0a};
byte L9[] = {0x0a,0x0a,0x0a,0x0a,0x0a,0x0a};
byte L10[] = {0x1b,0x69};

const int ledPin = 13;


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

  
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptCounter, FALLING);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, LOW);
  while (coinInserted == 1 && micros() - pulseTime < 4500000) {
    countWifiTime();
  }
  if (IRQcount > 0 && micros() - pulseTime >= 4500000) {
    startPrintSequence();
  }
}

void interruptCounter() {
  coinInserted = 1;
  pulseTime = micros();
}

void printThermal() {
  //Serial.write(L1, sizeof(L1));
  //Serial.write(L2, sizeof(L2));
  //Serial.write(L3, sizeof(L3));
  Serial.write(L4, sizeof(L4));
  Serial.write(L5, sizeof(L5));
  Serial.write(L6s, sizeof(L6s));
  Serial.write(response);
  Serial.write(L6e, sizeof(L6e));
  Serial.write(L7, sizeof(L7));
  Serial.write(L8s, sizeof(L8s));
  Serial.write(itoa(IRQcount/2, cstr, 10));
  Serial.write(L8e, sizeof(L8e));
  Serial.write(L9, sizeof(L9));
  Serial.write(L10, sizeof(L10));
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
    client.print(timestring);
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
      ///////////////////////
      digitalWrite(ledPin, HIGH);
      timeleft = IRQcount*1800;
      timestring = String(timeleft);
      getAccountName();
      printThermal();
      IRQcount = 0;
      coinInserted = 0;
      digitalWrite(ledPin, LOW);
}

void countWifiTime() {
    int buttonState = digitalRead(interruptPin);
    
    while (buttonState == 0) {
      LOWcount ++;
      buttonState = digitalRead(interruptPin);
      HIGHcount = 0;
    }
    
    while (buttonState == 1 && HIGHcount < 10 && LOWcount > 10) {
      HIGHcount ++;
      buttonState = digitalRead(interruptPin);
    }

    buttonState = digitalRead(interruptPin);

    if (buttonState == 0 && HIGHcount < 9) {
      LOWcount = 0;
      HIGHcount = 0;
    }

    if (LOWcount < 9 && HIGHcount < 9){
      LOWcount = 0;
      HIGHcount = 0;
    }

    if (LOWcount > 8 and HIGHcount > 8){
      LOWcount = 0;
      HIGHcount = 0;
      IRQcount ++;
    }
}




