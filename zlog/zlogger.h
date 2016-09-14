#ifndef _ZLOGGER_H_
#define _ZLOGGER_H_

#include <string>

class ZLogger {
public:
    ZLogger(std::string f = "");
    enum level { debug = 0, info = 1, warning = 2, error = 3, fatal = 4 };
    void log(ZLogger::level level, std::string message);
private:
    std::string level_str(ZLogger::level);
};

#endif // _ZLOGGER_H_