#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h> 
 
/*----- Declare all constant pin to be used ----*/
#define CE_PIN   9    //2.4GHz CE_pin of NRF24L01 module
#define CSN_PIN 10    //2.4GHz CSN_pin of NRF24L01 module

const uint64_t pipe = 0xE8E8F0F0E1LL; // This is the transmit pipe to communicate the two module
RF24 radio(CE_PIN, CSN_PIN);        // Activate  the Radio  
int joystick[7];  // 7 element array holding the Joystick readings

int motorspeed;
int motor1pin1 = 3;       // left motors 2
int motor1pin2 = 4;       // left motor 1
int motor2pin1 = 7;       // right motor 1
int motor2pin2 = 8;       // right motor 2
int EnableA = 5;        // enable pin for motor 1
int EnableB = 6;        // enable pin for motor 2

Servo panServo;         // the servo for panning
Servo tiltServo;        // the servo for tilting

void setup() { 
 Serial.begin(9600); /* Opening the Serial Communication */
 delay(1000);
 Serial.println("Nrf24L01 Receiver Starting");

 radio.begin();
 radio.setPALevel(RF24_PA_MIN);
 radio.setAutoAck(true);
 radio.openReadingPipe(1,pipe);
 radio.startListening();
 radio.powerUp();

 pinMode(motor1pin1, OUTPUT);
 pinMode(motor1pin2, OUTPUT);
 pinMode(motor2pin1, OUTPUT);
 pinMode(motor2pin2, OUTPUT);

 pinMode(EnableA, OUTPUT);
 pinMode(EnableB, OUTPUT);

 panServo.attach(A2);      // pan servo on A2
 tiltServo.attach(A3);     // tilt servo on A3
 panServo.write(90);       // sets pan servo to middle
 tiltServo.write(50);      // sets tilt servo to middle
}

void loop()   /****** MAIN LOOP: RUNS CONSTANTLY ******/
{
  if (radio.available())
  {
    // Read the data payload until we've received everything
       radio.read( joystick, sizeof(joystick) );
       nrf24l01();    // all the joystick events in this function
   }  else  {    
      Serial.println("No radio available");
      halt();     // if no radio transmission is received, the car stops
  }
}//--(end main loop )---

void nrf24l01() { 
      radio.powerUp();
      Serial.print("Y =");
      Serial.print(joystick[0]);
      Serial.print("\tX =");      
      Serial.print(joystick[1]);
 
      // motors for moving Forward or Backward
      if(joystick[0] >65) { motorspeed=joystick[0]+134; forward(); }
      if(joystick[0] <55) { motorspeed=255-joystick[0]; backward(); }
      if(joystick[0] >=55 && joystick[0] <=65) { halt();  motorspeed=0;}
      Serial.print("\tY-Speed: ");      
      Serial.print(motorspeed);
    
      // motors for moving Left or Right
      if(joystick[1] >60) { motorspeed = 255; turnleft(); }
      if(joystick[1] <=55) { motorspeed = 255; turnright(); }
      if(joystick[1] >=60 && joystick[1] <=65) { halt(); motorspeed=0; }
      Serial.print("\tX-Speed: ");      
      Serial.print(motorspeed);

      // place your additional joystick commands received from the TX
      //
 }

void backward() {
   analogWrite(EnableA, motorspeed);
   analogWrite(EnableB, motorspeed); 
   digitalWrite(motor1pin1, HIGH);
   digitalWrite(motor1pin2, LOW);

   digitalWrite(motor2pin1, HIGH);
   digitalWrite(motor2pin2, LOW);
}

void forward() {
   analogWrite(EnableA, motorspeed);
   analogWrite(EnableB, motorspeed);
   digitalWrite(motor1pin1, LOW);
   digitalWrite(motor1pin2, HIGH);

   digitalWrite(motor2pin1, LOW);
   digitalWrite(motor2pin2, HIGH);
}

void halt() {       // stops all motors
   digitalWrite(motor1pin1, LOW);
   digitalWrite(motor1pin2, LOW);

   digitalWrite(motor2pin1, LOW);
   digitalWrite(motor2pin2, LOW);
}

void turnleft() {
   digitalWrite(EnableA, HIGH);
   digitalWrite(EnableB, HIGH);
   digitalWrite(motor1pin1, HIGH);
   digitalWrite(motor1pin2, LOW);

   digitalWrite(motor2pin1, LOW);
   digitalWrite(motor2pin2, HIGH);
}

void turnright() {
   digitalWrite(EnableA, HIGH);
   digitalWrite(EnableB, HIGH);
   digitalWrite(motor1pin1, LOW);
   digitalWrite(motor1pin2, HIGH);

   digitalWrite(motor2pin1, HIGH);
   digitalWrite(motor2pin2, LOW);
}                                              


