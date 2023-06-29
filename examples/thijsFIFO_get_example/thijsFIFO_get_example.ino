
#define FIFO_DEBUG_SERIAL Serial


#include "thijsFIFO.h"


//// the get() function just uses an internal counter ('testFIFO.available') to make sure you only retrieve entries you haven't retrieved before
FIFO<uint16_t> testFIFO(5); // contructor arguments are array size and whether or not to initialize values to 0 (via memset)

void setup() {
  Serial.begin(115200);
  Serial.print("size: "); Serial.println(testFIFO.size);
  Serial.println();
  Serial.println("initial values:");
  for(size_t i=0; i<testFIFO.size; i++) {
    Serial.println(testFIFO[i]);
  }
  
  Serial.println();
  Serial.println("putting some things in:");
  size_t i=0;
  while(testFIFO.available < (testFIFO.size-1)) { // size-1 to set up for the '.available' example after this, this is not the norm
    Serial.println(testFIFO.put(i*10));
    i++;
  }
  Serial.println();
  Serial.print("available:"); Serial.println(testFIFO.available); //should not be filled yet
  Serial.println(testFIFO.put(69));
  Serial.print("available:"); Serial.println(testFIFO.available); //should be just filled
  Serial.println(testFIFO.put(420));
  Serial.print("available:"); Serial.println(testFIFO.available); //overwrote oldest value
  
  Serial.println();
  Serial.println("current values (from oldest to newest (First Out)):");
  while(testFIFO.available) {
    Serial.println(testFIFO.get());
  }

  Serial.println();
  Serial.println("out of available example (available == 0):");
  Serial.println(testFIFO.get()); //out of bounds example
  delay(10); //the esp32 needs time to print (log_e) error messages, but the program just keeps going, so wait for that to finish

}

void loop() {
  
}
