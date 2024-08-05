#include <EEPROM.h>

//Konfigurasi Sensor Flow Air
const int           flowSensorPin     = D4;
volatile int        pulseCount        = 0;
unsigned long       time1             = 0;
float               flowrate          = 0.0;
unsigned int        flowmL            = 0;
unsigned long       totalmL           = 0;
const unsigned long time_millis       = 1000;

//Konfigurasi EEPROM
#define EEPROM_SIZE 4
int Savewaterflow;

void ICACHE_RAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup(){
  Serial.begin(115200);
  pinMode(flowSensorPin, INPUT_PULLUP);
  digitalWrite(flowSensorPin, HIGH);
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING);
  EEPROM.begin(EEPROM_SIZE);
  Savewaterflow = EEPROM.read(0);
  EEPROM.get(0, Savewaterflow);
  if (isnan(Savewaterflow) || Savewaterflow < 0) {
    Savewaterflow = 0.0;
  }
  Serial.print("Total penggunaan air: ");
  Serial.println(Savewaterflow);
}

void loop(){
  delay(500);
  if ((millis() - time1) > time_millis) {
    detachInterrupt(digitalPinToInterrupt(flowSensorPin));
    flowrate = (float)pulseCount;
    pulseCount = 0;
    flowmL = (flowrate / 60) * 1000;
    totalmL += flowmL;
    EEPROM.put(0, Savewaterflow);
    Savewaterflow += flowrate;
    EEPROM.write(0, Savewaterflow);
    EEPROM.commit();
    Serial.print("Aliran Air (L/min): ");
    Serial.print(flowrate); 
    Serial.print(" L/min");
    Serial.print(" %% ");
    Serial.print("Aliran Air (mL): ");
    Serial.print(flowmL); 
    Serial.print(" mL");
    Serial.print(" %% ");
    Serial.print("Total Penggunaan Air: ");
    Serial.print(Savewaterflow);
    Serial.println(" mL");    
    time1 = millis();
    attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING);
  }
}