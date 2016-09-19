#include "logger.h"

int main(int, char*[])
{
  Logger logger("foo");
  logger.log(Logger::Level::debug, "Hello world");
}