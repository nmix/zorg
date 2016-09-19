#ifndef _ZLOG_H_
#define _ZLOG_H_

#include <string>

class ZLog {
public:
    ZLog(std::string f = "");
    enum class Level { debug = 0, info = 1, warning = 2, error = 3, fatal = 4 };
    void log(ZLog::Level level, std::string message);
private:
    std::string level_str(ZLog::Level);
};

#endif // _ZLOG_H_