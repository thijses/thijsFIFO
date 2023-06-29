
#define FIFO_DEBUG_SERIAL Serial


#include "thijsFIFO.h"


//// there are several ways to initialize:

FIFO<uint16_t> testFIFO(5); // contructor arguments are array size and whether or not to initialize values to 0 (via memset)
//FIFO<uint16_t> testFIFO(5, false); // default is 'true', which will set all bytes to 0

//FIFO<uint16_t> testFIFO(5, 64000); // contructor arguments are array size and initial value

//uint16_t existingArray[5] = {1,2,3,4,5}; // an existing array (note: the _cursor scrolls forwards, so _data contents are read backwards)
//FIFO<uint16_t> testFIFO((uint16_t*) &existingArray, 5); // contructor arguments are an existing array and its size

// note: you may get into trouble if you want to make FIFOs of bytes or booleans, as those can make 'ambigous/overloaded' arguments
//       it may help to specify clearly like "FIFO<uint8_t> testFIFO(5, (bool) 1);" instead of "FIFO<uint8_t> testFIFO(5, 1);

//// of course, you can also go multidimensional by making an array of FIFOs:
//FIFO<float> testFIFOarray[2] = {{5, true}, {7, true}}; //initialization of each individual FIFO is slightly tricky
//#define testFIFO testFIFOarray[0]     //this is just to make the rest of the example code behave the same, you can safely ignore this

//// you can also pass structs if you want (note: the rest of the example code is not prepared to work with this)
//struct exampStruct {
//  uint8_t singleByte = 123;
//  float vector[2] = {4.56, 7.89};
//};
//FIFO<exampStruct> testFIFO(5, false);

//// you could even go so far as to do:
//struct exampStruct : public Printable {
//  uint16_t _data[3] = {100, 200, 300};
//  inline uint16_t &operator[](size_t index) { //make struct [] able (just like FIFO)
//    return(_data[index]);
//  }
//  public:
//  size_t printTo(Print& p) const{ //this is all just extra floof, but it's nice to know it's possible, right :)
//    return(p.print("exampStruct{"+String(_data[0])+','+String(_data[1])+','+String(_data[2])+"}"));
//  }
//};
//FIFO<exampStruct> testFIFO(5, false);

// in my interpretation of a FIFO, the 0th index, so testFIFO[0] should return the OLDEST value
// you can also call FILO<>, which is exactly the same except obj[0] returns the NEWEST value

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
  for(size_t i=0; i<(testFIFO.size-1); i++) { // size-1 to set up for the '.filled' example after this, this is not the norm
    Serial.println(testFIFO.put(i*10));
  }
  Serial.print("filled:"); Serial.println(testFIFO.filled); //should not be filled yet
  Serial.println(testFIFO.put(69));
  Serial.print("filled:"); Serial.println(testFIFO.filled); //should be just filled
  Serial.println(testFIFO.put(420));
  Serial.print("filled:"); Serial.println(testFIFO.filled); //overwrote oldest value
  
  Serial.println();
  Serial.println("current values (from oldest to newest (First Out)):");
  for(size_t i=0; i<testFIFO.size; i++) {
    Serial.println(testFIFO[i]);
  }

  Serial.println();
  Serial.println("out of bounds example:");
  Serial.println(testFIFO[testFIFO.size]); //out of bounds example
  delay(10); //the esp32 needs time to print (log_e) error messages, but the program just keeps going, so wait for that to finish

  #ifdef testFIFO     //a little hack to make this only show up in the multidimensional example, this is not good code
    Serial.println();
    Serial.println("multidimensional example");
    for(size_t j=0; j<2; j++) {
      Serial.print("FIFO: "); Serial.println(j);
      for(size_t i=0; i<testFIFOarray[j].size; i++) {
        Serial.println(testFIFOarray[j][i]);
      }
    }
  #endif
}

void loop() {
  
}
