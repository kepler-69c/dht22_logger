#include "logger.h"

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>

std::string getISODate() {
  auto now = std::chrono::system_clock::now();
  return std::format("{:%F}", now);
}

std::string getTime() {
  auto now = std::chrono::system_clock::now();
  auto now_sec = std::chrono::floor<std::chrono::seconds>(now);
  return std::format("{:%H%M%S}", now_sec);
}

Logger::Logger(std::string dir)
    : dir(dir), calls_without_change{0}, last_temp{0.0f}, last_humidity{0.0f} {
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

  bool write_header = !std::filesystem::exists(path) || std::filesystem::file_size(path) == 0;
  file.open(path, std::ios::app);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open log file");
    return;
  }

  // if file didn't exist, write header
  if (write_header)
    file << "time,temp,humidity\n";
}

void Logger::log(float temp, float humidity) {
  // if date changed, open a new file
  if (!file.is_open() || getISODate() != date)
    open_file();
  // write changes
  file << getTime() << "," << temp << "," << humidity << "\n"
       << std::flush;
}

void Logger::log_on_change(float temp, float humidity) {
  // check if the values have changed enough; write periodically, even if the change is small
  if (abs(temp - last_temp) < MIN_CHANGE &&
      abs(humidity - last_humidity) < MIN_CHANGE &&
      calls_without_change++ < MAX_WITHOUT_CHANGE) {
    std::cout << "DEBUG: skipping log call" << std::endl;
    return;
  }

  log(temp, humidity);
  last_temp = temp;
  last_humidity = humidity;
  calls_without_change = 0;
}