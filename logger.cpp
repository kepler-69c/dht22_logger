#include "logger.h"

#include <chrono>
#include <filesystem>
#include <format>

std::string getISODate() {
  auto now = std::chrono::system_clock::now();
  return std::format("{:%F}", now);
}

std::string getTime() {
  auto now = std::chrono::system_clock::now();
  return std::format("{:%H%M%S}", now);
}

Logger::Logger(std::string dir)
    : calls_without_change{0}, last_temp{0.0f}, last_humidity{0.0f}, dir(dir) {
  open_file();
}

Logger::~Logger() {
  file.close();
}

void Logger::open_file() {
  date = getISODate();
  // close existing file
  if (file.is_open())
    file.close();

  std::filesystem::create_directories(dir);
  auto path = std::filesystem::path(dir) / (date + ".csv");

  bool exists = std::filesystem::exists(path);
  file.open(path, std::ios::app);

  // if file didn't exist, write header
  if (!exists)
    file << "time,temp,humidity\n";
}

void Logger::log(float temp, float humidity) {
  // if date changed, open a new file
  if (!file.is_open() || getISODate() != date)
    open_file();
  // write changes
  file << getTime() << "," << temp << "," << humidity << "\n";
}

void Logger::log_on_change(float temp, float humidity) {
  // check if the values have changed enough
  if (abs(temp - last_temp) < MIN_CHANGE || abs(humidity - last_humidity) < MIN_CHANGE)
    // write periodically, even if the change is small
    if (calls_without_change++ < MAX_WITHOUT_CHANGE)
      return;

  log(temp, humidity);
  last_temp = temp;
  last_humidity = humidity;
  calls_without_change = 0;
}