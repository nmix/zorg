
#include "zlogger.h"

int main(int, char*[])
{
    ZLogger logger = ZLogger();
    logger.log(ZLogger::Level::debug, "Debug message");
    logger.log(ZLogger::Level::info, "Info message");
    logger.log(ZLogger::Level::warning, "Warning message");
    logger.log(ZLogger::Level::error, "Error message");
    logger.log(ZLogger::Level::fatal, "Fatal message");
    return 0;
    // logger.log(ZLogger::Level::debug, (boost::format("Debug message # %d") % i).str() );
}
