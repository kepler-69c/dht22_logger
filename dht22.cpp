/*
Compile with
    g++ dht22.cpp dht22lib.cpp -lwiringPi -DDEBUG -o dht22
*/

#include "dht22lib.h"
#include "logger.h"

int main(int argc, char* argv[]) {
  // Parse inputs
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " pin" << "\n";
    return 1;
  }
  int pin;
  try {
    pin = std::stoi(argv[1]);
  } catch (std::invalid_argument) {
    std::cerr << "Invalid argument: " << argv[1] << "\n";
    return 1;
  }

  // Set up wiringPi to use the regular GPIO pin numbers (http://wiringpi.com/reference/setup/)
  // These are the ones labelled as BCM, on the map shown with command ```gpio readall```
  if (wiringPiSetupGpio() == -1) {
    std::cout << "wiringPi setup failed";
    return 1;
  }

  DhtSensor sensor{pin};
  Logger logger("logs");

  // Read from the sensor
  int delayMilliseconds = 5000;
  int delayOnFailureMilliseconds = 500;
  while (true) {
    sensor.read();
    // sensor.print();

    if (sensor.m_readType == GOOD) {
      logger.log_on_change(sensor.m_temperature, sensor.m_humidity);
      delay(delayMilliseconds);  // Wait between readings
    } else {
      delay(delayOnFailureMilliseconds);
    }
  }

  return (0);
}