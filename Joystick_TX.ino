/* Example: nRF24L01 
 
   1 - GND 
   2 - VCC 3.3V  
   3 - CE - Arduino pin 9
   4 - CSN - Arduino pin 10
   5 - SCK - Arduino pin 13
   6 - MOSI - Arduino pin 11
   7 - MISO - Arduino pin 12
   8 - UNUSED 
 
  You can used an Analog Joystick or 2 10K potentiometers 
 
   GND - Arduino GND 
   VCC - Arduino +5V
   X Pot - Arduino A0
   Y Pot - Arduino A1
 
/*----- Import all required Libraries -----*/
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
/*----- Declare all constant pin to be used ----*/
 
#define CE_PIN   7  // Uno= 9     // Mega = 48
#define CSN_PIN  8  // Uno= 10    // Mega = 53

#define JOYSTICK_X A0
#define JOYSTICK_Y A1
// define your additional joystick variable constants here

const uint64_t pipe = 0xE8E8F0F0E1LL; // This is the transmit pipe to communicate the two module 
/*-----Object Declaration ----*/ 
RF24 radio(CE_PIN, CSN_PIN); // Activate  the Radio
 
/*-----Declaration of Variables -----*/
int joystick[3];  // Six element array holding the Joystick readings
 
void setup()   
{
  Serial.begin(57600); /* Opening the Serial Communication */
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(pipe);
}//--(end setup )---
 
void loop()   /* Runs Continuously */
{
  joystick[0] = map(analogRead(JOYSTICK_Y), 0, 1023, 0, 179); // Reading Analog Y
  joystick[1] = map(analogRead(JOYSTICK_X), 0, 1023, 0, 179); // Reading Analog X

  // this is where you place your additional joystick array

  radio.write( joystick, sizeof(joystick) );

}//--(end main loop )---
