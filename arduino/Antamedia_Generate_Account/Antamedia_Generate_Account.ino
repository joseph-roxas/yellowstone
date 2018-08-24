/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

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
void setup() {
  // Open serial communications and wait for port to open:
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
  Serial.println("connecting...");
  
  //if (client.connect(server, 82)) {
  //  Serial.println("connected");
    // Make a HTTP request:
    //client.println("GET /search?q=arduino HTTP/1.1");
  //} else {
    // if you didn't get a connection to the server:
  //  Serial.println("connection failed");
  //}
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:

  while(!Serial.available()) {

  }
  char c = tolower(Serial.read());

  // Read any Serial data.
  do {
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);

  if (c == 'c') {
    function();
  } else if (c == 'p') {
    Serial.println();
    Serial.println((char*)response);
    Serial.println();
  } else {
    Serial.println(F("Invalid entry"));
  }
  
}

void function(){
  
  count = 0;
  i = 0;
  response[0] = (char)0;
 
  while (client.connect(server, 82)) {
    Serial.println("connected");
  }
 
  if (client.connected()) {
    // Make an API request:
    client.println("GET /generateaccounts?number=1&priceplan=7&pass=1JC8QBAQ037FNQL37JSBBSLP83EC60&type=2&sell=1&passlength=5&inactivity=0 HTTP/1.1");
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
      Serial.println(count);
      Serial.println((char*)response);
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      break;
    }
  }
}


