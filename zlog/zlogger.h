#ifndef _ZLOGGER_H_
#define _ZLOGGER_H_

#include <string>

class ZLogger {
public:
    ZLogger();
    enum class Level { debug, info, warning, error, fatal };

    void log(ZLogger::Level level, std::string message);

    
private:
    std::string filename;
};

#endif // _ZLOGGER_H_