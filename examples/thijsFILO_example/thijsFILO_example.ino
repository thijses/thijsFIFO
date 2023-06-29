
#define FIFO_DEBUG_SERIAL Serial


#include "thijsFIFO.h"


//// the FILO is largely the same, except values are retrieved newest to oldest (Last Out)
FILO<uint16_t> testFILO(5);

void setup() {
  Serial.begin(115200);
  Serial.print("size: "); Serial.println(testFIFO.size);
  Serial.println();
  Serial.println("initial values:");
  for(size_t i=0; i<testFILO.size; i++) {
    Serial.println(testFILO[i]);
  }
  
  Serial.println();
  Serial.println("putting some things in:");
  for(size_t i=0; i<(testFILO.size-1); i++) { // size-1 to set up for the '.filled' example after this, this is not the norm
    Serial.println(testFILO.put(i*10));
  }
  Serial.print("filled:"); Serial.println(testFILO.filled); //should not be filled yet
  Serial.println(testFILO.put(69));
  Serial.print("filled:"); Serial.println(testFILO.filled); //should be just filled
  Serial.println(testFILO.put(420));
  Serial.print("filled:"); Serial.println(testFILO.filled); //overwrote oldest value
  
  Serial.println();
  Serial.println("current values (from newest to newest (Last Out)):");
  for(size_t i=0; i<testFILO.size; i++) {
    Serial.println(testFILO[i]);
  }

  Serial.println();
  Serial.println("out of bounds example:");
  Serial.println(testFILO[testFILO.size]); //out of bounds example
  delay(10); //the esp32 needs time to print (log_e) error messages, but the program just keeps going, so wait for that to finish

}

void loop() {
  
}
