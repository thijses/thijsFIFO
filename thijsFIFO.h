/*
 *  thijsFIFO.h is a 'library' for simple First In First Out arrays
 *  
 *  i had to put all the functions in here, instead of a .cpp file, because of template<> complications (and my lack of skill in Cpp).
 *  i know that header files are not technically supposed to have substance (just headers), but it works for now.
 *  
 *  you could easily do this manually (or even with a struct or something),
 *  but this has fancy [] support and should be reasonably fast (i think)
 * 
 *  Created by Thijs van Liempd on 27/06/2021.
 */

#ifndef THIJS_FIFO
#define THIJS_FIFO

#include "Arduino.h"

//#define FIFO_DEBUG_SERIAL Serial

template<typename T> struct _FI {
public: //all variables are public, but some are marked with a suggestive _ (underscore) to indicate unsafety
  T* _data; //array is intialized in constructor
  size_t size; //set in constructor
  volatile size_t _cursor=0; //the 'cursor' holds the OLDEST index (which is overwritten at the next insert)
  volatile size_t available=0; // only used for get()
  volatile bool filled = false;
  _FI(T* existingArray, size_t arraySize) : size(arraySize) {
    _data = existingArray;
  }
  _FI(size_t arraySize, const bool initZero=true) : size(arraySize) { //this does mean you can't make a FIFO of (const?) booleans
    _data = new T[arraySize];
    if(initZero) {
      memset(_data, 0, sizeof(T)*arraySize);
    }
  }
  _FI(size_t arraySize, T initialValue) : size(arraySize) {
    _data = new T[arraySize];
    for(size_t i=0; i<arraySize; i++) {
      _data[i] = initialValue;
    }
  }
  ~_FI() { delete []_data; }

  T put(T dataToInsert) {
    _data[_cursor] = dataToInsert;
    _cursor++;
    if(available < size) { available++; } // else { Serial.println("buffer overflow?"); }
    //_cursor = (_cursor<size) ? _cursor : 0; //rollover done one way
    if(_cursor >= size) { // rollover done this way just allows for the 'filled' bool to be set
      _cursor = 0;
      filled = true;
    }
    return(dataToInsert);
  }
  //size_t& available() {return(available);} // i think people (my future self) will understand that 'available' is just a counter, which doesnt need a function
};

template<typename T> struct FIFO : public _FI<T> { // First In Last Out
public:
  using _FI<T>::_FI;
  T& operator[](size_t index) { // the 0th index returns the OLDEST value, the highest index (size-1) returns the newest value
    if(index >= _FI<T>::size) {
      #if defined(ARDUINO_ARCH_ESP32)
        log_e("FIFO out of bounds");
      #elif defined(FIFO_DEBUG_SERIAL)
        FIFO_DEBUG_SERIAL.println("FIFO out of bounds");
      #endif
    }
    size_t shiftedIndex = (index + _FI<T>::_cursor) % _FI<T>::size;
    return(_FI<T>::_data[shiftedIndex]);
  }

  T& get() { // get newly added (unread) values from oldest to newest (but only once!). check FIFO.available > 0 beforehand!
    if(!_FI<T>::available) {
      #if defined(ARDUINO_ARCH_ESP32)
        log_e("FIFO.get() when FIFO.available == 0");
      #elif defined(FIFO_DEBUG_SERIAL)
        FIFO_DEBUG_SERIAL.println("FIFO.get() when FIFO.available == 0");
      #endif
      _FI<T>::available++; // just so it doesn't crash
    }
    size_t shiftedIndex = ((_FI<T>::size - _FI<T>::available) + _FI<T>::_cursor) % _FI<T>::size;
    _FI<T>::available--;
    return(_FI<T>::_data[shiftedIndex]);
  }
};

template<typename T> struct FILO : public _FI<T> { // First In Last Out
public:
  using _FI<T>::_FI;
  T& operator[](size_t index) { // the 0th index returns the NEWEST value, the highest index (size-1) returns the oldest value
    if(index >= _FI<T>::size) {
      #if defined(ARDUINO_ARCH_ESP32)
        log_e("FILO out of bounds");
      #elif defined(FIFO_DEBUG_SERIAL)
        FIFO_DEBUG_SERIAL.println("FILO out of bounds");
      #endif
      //just consider it rollover (doesnt make sense, but i need a way to deal with it and the compiler won't let me return NULL)
      index = index % _FI<T>::size;
    }
    //size_t shiftedIndex = (index < _FI<T>::_cursor) ? (_FI<T>::_cursor - index - 1) : (_FI<T>::size + _FI<T>::_cursor - index - 1); // same thing (i don't know if this is faster than modulo)
    size_t shiftedIndex = (_FI<T>::size + _FI<T>::_cursor - index - 1) % _FI<T>::size; // relies on index being < size to maintain unsigned values
    return(_FI<T>::_data[shiftedIndex]);
  }
  
  T& get() { // get newly added (unread) values from newest to oldest (but only once!). check FILO.available > 0 beforehand!
    if(!_FI<T>::available) {
      #if defined(ARDUINO_ARCH_ESP32)
        log_e("FILO.get() when FIFO.available == 0");
      #elif defined(FIFO_DEBUG_SERIAL)
        FIFO_DEBUG_SERIAL.println("FILO.get() when FILO.available == 0");
      #endif
      _FI<T>::available++; // just so it doesn't crash
    }
    size_t shiftedIndex = (_FI<T>::available + _FI<T>::_cursor - 1) % _FI<T>::size;
    _FI<T>::available--;
    return(_FI<T>::_data[shiftedIndex]);
  }
};

#endif