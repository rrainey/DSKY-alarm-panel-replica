/*
Copyright (c) 2020, Riley Rainey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <Arduino.h>
#include <Wire.h>

#define DRIVER_VERSION 1
#define I2C_ADDRESS    0x38

#define PIN_RED_LED  13

// consult variant.cpp for the dskymatic_m0 board definition
// to see how digital pins were added

#define PA_PIN(x) (23+(x))

#define LMAP_IGNORE 255
#define LMAP_SIZE 14

/*
 * Output definitions valid for V3 and later board
 */
const uint8_t l_map[LMAP_SIZE] = {
  PA_PIN(2),
  PA_PIN(3),
  PA_PIN(4),
  PA_PIN(5),
  PA_PIN(19),
  PA_PIN(22), // lamp not populated on board
  PA_PIN(23), // lamp not populated on board
  PA_PIN(27),
  PA_PIN(11),
  PA_PIN(14),
  PA_PIN(15),
  PA_PIN(16),
  PA_PIN(17),
  PA_PIN(18)
};

uint16_t last_command = 0;

void updateLamps(uint16_t command) {
  
  uint16_t check_bit = 0x8000;
  
  for (int i=0; i<LMAP_SIZE; ++i) {

    uint16_t changed = last_command ^ command;
    
    if (l_map[i] != LMAP_IGNORE) {
      if ((changed & check_bit) != 0) {
        digitalWrite(l_map[i], (command & check_bit) ? HIGH : LOW );
      }
    }
    check_bit >>= 1;
  }

  last_command = command;
}

void receiveEvent(int count)
{
  uint16_t command = 0;
  int i = 0;
  while(Wire.available())
  {
    if (i++ < 2) {
      command = (command << 8) || Wire.read();
    }
  }
  updateLamps(command);
}

void setup() 
{
  pinMode(PIN_RED_LED, OUTPUT);
  digitalWrite(PIN_RED_LED, HIGH);

  /*
   * Turn off all lamps
   */
  for(int i=0; i<LMAP_SIZE; ++i) {
    if (l_map[i] != LMAP_IGNORE) {
      pinMode( l_map[i], OUTPUT );
      digitalWrite( l_map[i], LOW );
    }
  }

  last_command = 0;

  /* 
   *  Connect to I2C Bus
   */
  Wire.begin( I2C_ADDRESS );
  Wire.onReceive( receiveEvent );
}

void loop() {
}
