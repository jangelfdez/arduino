/* 
  Connections

  SCL -> SCL BME280
  SDA -> SDA BME280
  D7 -> PWM Garage Door
*/

#include <BME280I2C.h>
#include <Wire.h>
#include <Servo.h>

#define SERIAL_BAUD 115200
#define GARAGE_SERVO_PORT 11
#define EXTERNAL_LIGHTS_INPUT_PORT A0
#define EXTERNAL_LIGHTS_OUTPUT_PORT 2
#define EXTERNAL_LIGHTS_THRESHOLD 500
#define MOVEMENT_SENSOR_INPUT_PORT 3
#define MOVEMENT_SENSOR_OUTPUT_PORT 5
#define BLIND_CONTROL_ORANGE 8
#define BLIND_CONTROL_GREEN 7
#define BLIND_CONTROL_YELLOW 12
#define BLIND_SERVO_PORT 10
#define ON 1
#define OFF 0

BME280I2C bme;
Servo garage_servo;
Servo blind_servo;

int garage_servo_angle = 0;


// 0 -> closed; 1 -> open
bool garage_door_status = 0;
bool blind_status = 0;
// 0 -> no movement; 1 -> movement
bool movement_sensor_status = 0;
// 0 -> off; 1 -> on
bool outside_lights_status = 0;


void setup() {
  setup_serial_comm();
  setup_i2c_comm();
  setup_bme280_sensor();
  setup_garage_servo();
  setup_blind_servo();
  setup_external_lights();
  setup_movement_sensor();
}

void loop() {
  check_outside_lights();
  check_movement_sensor();

  if (Serial.available() > 0) {
    String incoming_command = Serial.readString();
    //Serial.print("RX Command > ");

    switch (incoming_command.toInt()) {
      case 0:
        {
          //Serial.println("0 [Read Garage Door Status]");
          Serial.println(String(garage_door_status));
          break;
        }
      case 1:
        {
          //Serial.println("1 [Open Garage Door]");
          open_garage_door();
          Serial.println("OK");
          break;
        }
      case 2:
        {
          //Serial.println("2 [Close Garage Door]");
          close_garage_door();
          Serial.println("OK");
          break;
        }
      case 3:
        {
          //Serial.println("3 [Read House Temperature]");
          float temp = read_house_temperature();
          Serial.println(String(temp));
          break;
        }
      case 4:
        {
          //Serial.println("4 [Read House Humidity]");
          float hum = read_house_humidity();
          Serial.println(String(hum));
          break;
        }
      case 5:
        {
          //Serial.println("5 [Read Movement Sensor Status]");
          movement_sensor_status = digitalRead(MOVEMENT_SENSOR_INPUT_PORT);
          Serial.println(movement_sensor_status);
          break;
        }
      case 6:
        {
          //Serial.println("6 [Read Outside Lights Status]");
          Serial.println(String(outside_lights_status));
          break;
        }
      case 7:
        {
          //Serial.println("7 [Read Blind Status]");
          blind_status = String(blind_status);
          Serial.println(blind_status);
          break;
        }
      case 8:
        {
          //Serial.println("8 [Open Blind]");
          open_blind();
          Serial.println("OK");
          break;
        }
      case 9:
        {
          //Serial.println("9 [Close Blind]");
          close_blind();
          Serial.println("OK");
          break;
        }
      default:
        {
          Serial.println("NOK");
          break;
        }
    }
  }
}


void setup_serial_comm() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("Bringing up the serial communication with the PC");
  while (!Serial) {}  // Wait
}

void setup_i2c_comm() {
  Wire.begin();
}

void setup_bme280_sensor() {
  int retries = 3;

  while (retries) {
    if (bme.begin()) {
      switch (bme.chipModel()) {
        case BME280::ChipModel_BME280:
          Serial.println("Found BME280 sensor! Success.");
          break;
        case BME280::ChipModel_BMP280:
          Serial.println("Found BMP280 sensor! No Humidity available.");
          break;
        default:
          Serial.println("Found UNKNOWN sensor! Error!");
      }
      retries = 0;
    } else {
      Serial.println("Could not find BME280 sensor!");
      retries--;
      delay(1000);
    }
  }
}

void setup_garage_servo() {
  //garage_servo.attach(GARAGE_SERVO_PORT);
  //garage_servo.write(0);
}

void setup_blind_servo() {
  //blind_servo.attach(BLIND_SERVO_PORT);
  pinMode(BLIND_CONTROL_YELLOW, OUTPUT);
  pinMode(BLIND_CONTROL_ORANGE, OUTPUT);
  pinMode(BLIND_CONTROL_GREEN, OUTPUT);
  //digitalWrite(BLIND_CONTROL_ORANGE, LOW);
  //digitalWrite(BLIND_CONTROL_YELLOW, LOW);
}

void setup_external_lights() {
  pinMode(EXTERNAL_LIGHTS_INPUT_PORT, INPUT);
  pinMode(EXTERNAL_LIGHTS_OUTPUT_PORT, OUTPUT);
}

void setup_movement_sensor() {
  pinMode(MOVEMENT_SENSOR_INPUT_PORT, INPUT);
  pinMode(MOVEMENT_SENSOR_OUTPUT_PORT, OUTPUT);
  digitalWrite(MOVEMENT_SENSOR_OUTPUT_PORT, LOW);
}

void check_outside_lights() {
  int input = analogRead(EXTERNAL_LIGHTS_INPUT_PORT);
  if (input < EXTERNAL_LIGHTS_THRESHOLD) {
    digitalWrite(EXTERNAL_LIGHTS_OUTPUT_PORT, HIGH);
    outside_lights_status = ON;
  } else {
    digitalWrite(EXTERNAL_LIGHTS_OUTPUT_PORT, LOW);
    outside_lights_status = OFF;
  }
  return;
}

void open_garage_door() {
  garage_servo.attach(GARAGE_SERVO_PORT);
  while (garage_servo_angle < 180) {
    garage_servo.write(garage_servo_angle);
    delay(5);
    garage_servo_angle++;
  }
  garage_door_status = ON;
  garage_servo.detach();
  return;
}

void close_garage_door() {
  garage_servo.attach(GARAGE_SERVO_PORT);
  while (garage_servo_angle > 0) {
    garage_servo.write(garage_servo_angle);
    delay(5);
    garage_servo_angle--;
  }
  garage_door_status = OFF;
  garage_servo.detach();
  return;
}

void open_blind() {
  blind_servo.attach(BLIND_SERVO_PORT);
  blind_servo.write(180);
  digitalWrite(BLIND_CONTROL_ORANGE, HIGH);
  digitalWrite(BLIND_CONTROL_GREEN, LOW);
  blind_status = ON;
  delay(1200);
  digitalWrite(BLIND_CONTROL_ORANGE, LOW);
  digitalWrite(BLIND_CONTROL_YELLOW, LOW);
  blind_servo.detach();
  return;
}

void close_blind() {
  blind_servo.attach(BLIND_SERVO_PORT);
  blind_servo.write(180);
  digitalWrite(BLIND_CONTROL_ORANGE, HIGH);
  digitalWrite(BLIND_CONTROL_GREEN, HIGH);
  delay(1200);
  blind_status = OFF;
  digitalWrite(BLIND_CONTROL_ORANGE, LOW);
  digitalWrite(BLIND_CONTROL_YELLOW, LOW);
  blind_servo.detach();
  return;
}

float read_house_temperature() {
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  return bme.temp(tempUnit);
}

float read_house_humidity() {
  //BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  return bme.hum();
}

void check_movement_sensor() {
  int input = digitalRead(MOVEMENT_SENSOR_INPUT_PORT);
  if (input == LOW) {
    digitalWrite(MOVEMENT_SENSOR_OUTPUT_PORT, HIGH);
    delay(500);
  } else {
    digitalWrite(MOVEMENT_SENSOR_OUTPUT_PORT, LOW);
    delay(500);
  }
  return;
}