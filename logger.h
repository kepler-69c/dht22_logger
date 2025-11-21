#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

#define MIN_CHANGE 0.2          // min change to save
#define MAX_WITHOUT_CHANGE 300  // 5 min max without save

class Logger {
 public:
  Logger(std::string dir);
  ~Logger();
  void log_on_change(float temp, float humidity);

 private:
  void open_file();
  void log(float temp, float humidity);

  std::string dir;
  std::string date;
  std::ofstream file;
  size_t calls_without_change;
  float last_temp;
  float last_humidity;
};

#endif