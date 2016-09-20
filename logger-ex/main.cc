#include "logger.h"

int main(int, char*[])
{
  Logger logger("LOGGER-EX");
  logger.log(Logger::Level::debug, "Debug message");
  logger.log(Logger::Level::info, "Info message");
  logger.log(Logger::Level::warning, "Warning message");
  logger.log(Logger::Level::error, "Error message");
  logger.log(Logger::Level::fatal, "Fatal message");
}