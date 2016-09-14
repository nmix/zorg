
#include "zlogger.h"

#include <boost/format.hpp>

int main(int, char*[])
{
    ZLogger logger = ZLogger("foo_0.log");
    logger.log(ZLogger::level::debug, "Debug message");
    logger.log(ZLogger::level::info, "Info message");
    logger.log(ZLogger::level::warning, "Warning message");
    logger.log(ZLogger::level::error, "Error message");
    logger.log(ZLogger::level::fatal, "Fatal message");
    return 0;
    // logger.log(ZLogger::level::debug, (boost::format("Debug message # %d") % i).str() );
}
