#include <Servo.h>
/*
  NOTE:
  - Defining the pins for the components.
*/
#define IR_LED_LEFT 10
#define IR_RECEIVER_LEFT 11

#define IR_LED_MID 6
#define IR_RECEIVER_MID 7

#define IR_LED_RIGHT 2
#define IR_RECEIVER_RIGHT 3

#define SERVO_1 13
#define SERVO_2 12

Servo servo_1;
Servo servo_2;

int ROTATION_SPEED = 200;
const int STOP_VALUE = 1510;

void setup()
{
  /*
    NOTE:
    - Calibrating the IR sensors and LEDs.
  */
  pinMode(IR_LED_LEFT, OUTPUT);
  pinMode(IR_RECEIVER_LEFT, INPUT);

  pinMode(IR_LED_MID, OUTPUT);
  pinMode(IR_RECEIVER_MID, INPUT);

  pinMode(IR_LED_RIGHT, OUTPUT);
  pinMode(IR_RECEIVER_RIGHT, INPUT);

  /*
    NOTE:
    - We also want to print out to keep track
    of the values of the sensors at any
    given point.
  */
  Serial.begin(9600);

  /*
    NOTE:
    - Now, we're attaching the continuous
    servos onto pins 12 and 13.
  */
  servo_1.attach(SERVO_1);
  servo_2.attach(SERVO_2);

  /*
    NOTE:
    - In the first reference, it does
    say the value 1500 is when the servo stops
    rotating.
  */
  servo_1.writeMicroseconds(STOP_VALUE);
  servo_2.writeMicroseconds(STOP_VALUE);
}

void move(int speed) {
  /*
    NOTE:
    - If `speed` is positive then the vehicle
    moves in a certain direction and if `speed`
    is negative then the vehicle moves in the
    opposite direction.
  */
  const unsigned int STOP_VALUE_LEFT = 1510;
  const unsigned int STOP_VALUE_RIGHT = 1510;

  const unsigned int CALIBRATION_VALUE_LEFT = 8;

  servo_1.writeMicroseconds(STOP_VALUE_LEFT - speed - CALIBRATION_VALUE_LEFT);
  servo_2.writeMicroseconds(STOP_VALUE_RIGHT + speed);
}

void stop() {
  const unsigned int STOP_VALUE_LEFT = 1510;
  const unsigned int STOP_VALUE_RIGHT = 1510;
  servo_1.writeMicroseconds(STOP_VALUE_LEFT);
  servo_2.writeMicroseconds(STOP_VALUE_RIGHT);
}

void loop() {
  /*
    VALUE-READING SECTION
  */
  tone(IR_LED_LEFT, 38000, 8);
  delay(1);
  int left_sensor = !digitalRead(IR_RECEIVER_LEFT);
  delay(1);

  tone(IR_LED_MID, 38000, 8);
  delay(1);
  int front_sensor = !digitalRead(IR_RECEIVER_MID);
  delay(1);

  tone(IR_LED_RIGHT, 38000, 8);
  delay(1);
  int right_sensor = !digitalRead(IR_RECEIVER_RIGHT);
  delay(1);

  /*
      LOGIC SECTION
  */
  if (left_sensor && right_sensor && !front_sensor) {
    /*
      NOTE:
      - Moving straight
    */
    Serial.println("A");
    move(100);
    return;
  }
  if (front_sensor && right_sensor && !left_sensor) {
    Serial.println("B");
    /*
      NOTE:
      - Moving forward a little bit.
    */
    move(200);
    delay(370);
    /*
      NOTE:
      - Turning left.
    */
    servo_1.writeMicroseconds(1510 - 200);
    servo_2.writeMicroseconds(1510);
    delay(1125);

    /*
      NOTE:
      - Moving forward a little bit out of the opening.
    */
    move(100);
    delay(330);
    return;
  }

  if (front_sensor && !right_sensor && left_sensor) {
    Serial.println("C");
    /*
      NOTE:
      - Moving forward a little bit.
    */
    move(200);
    delay(370);

    /*
      NOTE:
      - Turning right
    */
    servo_1.writeMicroseconds(1510);
    servo_2.writeMicroseconds(1510 + 200);
    delay(1135);

    /*
      NOTE:
      - Move forward a little bit out of the opening.
    */
    move(100);
    delay(330);

    return;
  }
  if (!front_sensor && !right_sensor && !left_sensor) {
    stop();
  }
}
