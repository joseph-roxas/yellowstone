#include <stdlib.h>

double x;
String xt;

byte L1[] = {0x0a,0x1d,0x21,0x00};
byte L2[] = {0x1b,0x40,0x1b,0x61,0x01,0x1d,0x21,0x00};
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

void setup() {
  Serial.begin(9600);
}

void loop() {

  while(!Serial.available()) {

  }
  char c = tolower(Serial.read());


  // Read any Serial data.
  do {
  //  delay(10);
  } while (Serial.available() && Serial.read() >= 0);
  
  if (c == 'c') {
    printThermal();
  } else {
    Serial.println(F("Invalid entry"));
  }


}


void printThermal() {
  Serial.write(L1, sizeof(L1));
  Serial.write(L2, sizeof(L2));
  Serial.write("Electric Meter Reading");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("Month of April 2018");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("From March 20, 2018");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("To April 20, 2018");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("Meter Number: 2222111");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("Previous Reading: 12.2 KWHrs");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("Present Reading: 25.0 KWHrs");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("Consumption: 12.8 KWHrs");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("Price per KWHr: P13.54/KWHr");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("Total Bill");
  Serial.write(L1, sizeof(L1));
  Serial.write(L6s, sizeof(L6s));
  Serial.write("P173.34");
  Serial.write(L6e, sizeof(L6e));
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("Payment Due: April 30, 2018");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write("Thank You!");
  Serial.write(L1, sizeof(L1));
  Serial.write("God Bless You Always.");
  Serial.write(L1, sizeof(L1));
  Serial.write(L1, sizeof(L1));
  Serial.write(L9, sizeof(L9));
  Serial.write(L10, sizeof(L10));
}







