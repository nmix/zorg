
#include "zlogger.h"

#include <boost/format.hpp>

int main(int, char*[])
{

    ZLogger logger = ZLogger();
    int i = 0;
    while (i < 3) {
        logger.log(ZLogger::Level::debug, (boost::format("Log message # %d") % i).str() );
        i += 1;
    }

    return 0;
}
