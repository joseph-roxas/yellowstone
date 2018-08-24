const byte interruptPin = 2;
volatile int IRQcount = 0;
volatile int LOWcount = 0;
volatile int HIGHcount =0;
volatile int newInsertedCoin = 0;
volatile int coinInserted = 0;
volatile unsigned long pulseTime;



void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptCounter, FALLING);

}

void loop() {
  
  while (coinInserted == 1 && millis() - pulseTime < 3500) {
  int buttonState = digitalRead(interruptPin);

  while (buttonState == 0) {
    Serial.println("Php 1.00");
    LOWcount ++;
    buttonState = digitalRead(interruptPin);
    HIGHcount = 0;
  }

  while (buttonState == 1 && HIGHcount < 10 && LOWcount > 10) {
    Serial.println("Php 2.00");
    HIGHcount ++;
    buttonState = digitalRead(interruptPin);
  }
  
  buttonState = digitalRead(interruptPin);
  
  if (buttonState == 0 && HIGHcount < 9) {
    Serial.println("Php 2.50");
    LOWcount = 0;
    HIGHcount = 0;
  }
  
  if (LOWcount < 9 && HIGHcount < 9){
    Serial.println("Php 3.00");
    LOWcount = 0;
    HIGHcount = 0;
    coinInserted = 0;
  }

  if (LOWcount > 8 and HIGHcount > 8){
    Serial.println("Php 5.0000000");
    coinInserted = 0;
    LOWcount = 0;
    HIGHcount = 0;
    IRQcount ++;
  }
  
  }

  if (IRQcount > 0 && millis() - pulseTime > 3500) {
    Serial.println(IRQcount);
    IRQcount = 0;
  }

}

void interruptCounter() {
  coinInserted = 1;
  pulseTime = millis();
}
