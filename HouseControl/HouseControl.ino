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
#define GARAGE_SERVO_PORT 7

BME280I2C bme;
Servo garage_servo;


// 0 -> closed; 1 -> open
bool garage_door_status = 0;
// 0 -> no movement; 1 -> movement
bool movement_sensor_status = 0;
// 0 -> off; 1 -> on
bool outside_lights_status = 0;


void setup() {
  setup_serial_comm();
  setup_i2c_comm();
  setup_bme280_sensor();
  setup_garage_servo();
}

void loop() {
  check_outside_lights();
  check_movement_sensor();
  
  if (Serial.available() > 0) {
    String incoming_command = Serial.readString();
    Serial.print("RX Command > ");

    switch (incoming_command.toInt()) {
      case 0:
        {
          Serial.println("0 [Read Garage Door Status]");
          Serial.println("-> Garage door status is " + String(garage_door_status));
          break;
        }
      case 1:
        {
          Serial.println("1 [Open Garage Door]");
          open_garage_door();
          Serial.println("-> Garage door is opened");
          break;
        }
      case 2:
        {
          Serial.println("2 [Close Garage Door]");
          close_garage_door();
          Serial.println("-> Garage door is closed");
          break;
        }
      case 3:
        {
          Serial.println("3 [Read House Temperature]");
          float temp = read_house_temperature();
          Serial.println("-> House temperature: " + String(temp));
          break;
        }
      case 4:
        {
          Serial.println("4 [Read House Humidity]");
          float hum = read_house_humidity();
          Serial.println("-> House humidity: " + String(hum));
          break;
        }
      case 5:
        {
          Serial.println("5 [Read Movement Sensor Status]");
          Serial.println("-> Movement sensor status is " + String(garage_door_status));
          break;
        }
      case 6:
        {
          Serial.println("6 [Read Outside Lights Status]");
          Serial.println("-> Outside light status is " + String(outside_lights_status));
          break;
        }
      default: {
          Serial.println("No command associated with that value");
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
  while (!bme.begin()) {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

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
}

void setup_garage_servo() {
  garage_servo.attach(GARAGE_SERVO_PORT);
}

void check_outside_lights() {
  return;
}

void open_garage_door() {
  return;
}

void close_garage_door() {
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
  return;
}