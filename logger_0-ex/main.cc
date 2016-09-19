
#include "zlog.h"

int main(int, char*[])
{
    ZLog logger;
    logger.log(ZLog::Level::debug, "Debug message");
    logger.log(ZLog::Level::info, "Info message");
    logger.log(ZLog::Level::warning, "Warning message");
    logger.log(ZLog::Level::error, "Error message");
    logger.log(ZLog::Level::fatal, "Fatal message");
    return 0;
    // logger.log(ZLogger::Level::debug, (boost::format("Debug message # %d") % i).str() );
}
