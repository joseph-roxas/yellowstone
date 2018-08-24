const byte interruptPin = 2;
volatile int IRQcount = 0;
volatile int LOWcount = 0;
volatile int HIGHcount =0;
volatile int newInsertedCoin = 0;
volatile int coinInserted = 0;
volatile unsigned long pulseTime;
char cstr[2];
char pass[7] = "DZ1LU";

byte L1[] = {0x0a};
byte L2[] = {0x1b,0x40,0x1b,0x61,0x01,0x1d,0x21,0x10,0x57,0x49,0x46,0x49,0x56,0x45,0x0d,0x0a};
byte L3[] = {0x0a};
byte L4[] = {0x1b,0x40,0x1b,0x61,0x01,0x50,0x6c,0x65,0x61,0x73,0x65,0x20,0x6c,0x6f,0x67,0x69,0x6e,0x0d,0x0a};
byte L5[] = {0x0a};
byte L6s[] = {0x1b,0x40,0x1b,0x61,0x01,0x1d,0x21,0x11};
byte L6e[] = {0x0d,0x0a};
byte L7[] = {0x0a};
byte L8s[] = {0x1b,0x40,0x1b,0x61,0x01,0x54,0x69,0x6d,0x65,0x3a,0x20};
byte L8e[] = {0x20,0x48,0x6f,0x75,0x72,0x2f,0x73,0x0d,0x0a};
byte L9[] = {0x0a,0x0a,0x0a,0x0a,0x0a,0x0a};
byte L10[] = {0x1b,0x69};

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptCounter, FALLING);
}

void loop() {
  

  while (coinInserted == 1 && millis() - pulseTime < 3500) {
  int buttonState = digitalRead(interruptPin);

  while (buttonState == 0) {
    //Serial.println("Php 1.00");
    LOWcount ++;
    buttonState = digitalRead(interruptPin);
    HIGHcount = 0;
  }

  while (buttonState == 1 && HIGHcount < 10 && LOWcount > 10) {
    //Serial.println("Php 2.00");
    HIGHcount ++;
    buttonState = digitalRead(interruptPin);
  }
  
  buttonState = digitalRead(interruptPin);
  
  if (buttonState == 0 && HIGHcount < 9) {
    //Serial.println("Php 2.50");
    LOWcount = 0;
    HIGHcount = 0;
  }
  
  if (LOWcount < 9 && HIGHcount < 9){
    //Serial.println("Php 3.00");
    LOWcount = 0;
    HIGHcount = 0;
  }

  if (LOWcount > 8 and HIGHcount > 8){
    //Serial.println("Php 5.0000000");
    //coinInserted = 0;
    LOWcount = 0;
    HIGHcount = 0;
    IRQcount ++;
  }
  
  }

  if (IRQcount > 0 && millis() - pulseTime > 3500) {
    //Serial.println(IRQcount);
    printThermal();
    IRQcount = 0;
    coinInserted = 0;
  }

}

void interruptCounter() {
  coinInserted = 1;
  pulseTime = millis();
}

void printThermal() {
  Serial.write(L1, sizeof(L1));
  Serial.write(L2, sizeof(L2));
  Serial.write(L3, sizeof(L3));
  Serial.write(L4, sizeof(L4));
  Serial.write(L5, sizeof(L5));
  Serial.write(L6s, sizeof(L6s));
  Serial.write(pass);
  Serial.write(L6e, sizeof(L6e));
  Serial.write(L7, sizeof(L7));
  Serial.write(L8s, sizeof(L8s));
  Serial.write(itoa(IRQcount/2, cstr, 10));
  Serial.write(L8e, sizeof(L8e));
  Serial.write(L9, sizeof(L9));
  Serial.write(L10, sizeof(L10));
}


