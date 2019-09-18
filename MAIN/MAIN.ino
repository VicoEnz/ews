#include "Adafruit_VL53L0X.h"  

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 3; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
  
void flow (){
  flow_frequency++;
}

void setupwaterflow(){
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  attachInterrupt(0, flow, RISING); // Setup Interrupt
  sei(); // Enable interrupts
  currentTime = millis();
  cloopTime = currentTime;
}

void setupdistance(){
  // wait until serial port opens for native USB devices  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (! Serial) {
    delay(1);
  }

  Serial.println("Setting up water speed module");
  setupwaterflow();

  Serial.println("Setting up distance module");
  setupdistance();
}

float getwaterVol() {
  currentTime = millis();
      if(currentTime >= (cloopTime + 100)){
        cloopTime = currentTime; // Updates cloopTime
        // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
        l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
        flow_frequency = 0; // Reset Counter
        Serial.print(l_hour, DEC); // Print litres/hour
        Serial.println(" L/hour");
        }
}
    float getdistance()
    {
      VL53L0X_RangingMeasurementData_t measure;
        
      Serial.print("Reading a measurement... ");
      lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    
      if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
      } else {
        Serial.println(" out of range ");
      }
        delay(100);{
      }
}

  void loop() {
    // put your main code here, to run repeatedly:
  getdistance();
  getwaterVol();
  delay(1000);
  }
