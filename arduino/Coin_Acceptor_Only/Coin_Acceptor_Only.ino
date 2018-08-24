const byte interruptPin = 2;
volatile int IRQcount;
volatile int newInsertedCoin = 0;
volatile int coinInserterd = 0;
volatile unsigned long pulseTime;



void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptCounter, RISING);
}

void loop() {
  
  while (IRQcount > 1 && millis() - pulseTime < 3000) {
  Serial.println(IRQcount);
  newInsertedCoin = IRQcount;
  }
  
  while (newInsertedCoin > 0) {
  Serial.print("Php ");
  Serial.println(newInsertedCoin*5/2);
  newInsertedCoin = 0;
  IRQcount=0;
  }

}

void interruptCounter() {
  IRQcount++;
  pulseTime = millis();
}
