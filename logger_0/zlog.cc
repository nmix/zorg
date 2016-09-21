#include "zlog.h"

// --- BOOST
#define BOOST_LOG_DYN_LINK // http://stackoverflow.com/questions/18881602/boost-logger-linking-issue
// ---
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/move/utility.hpp>
// --- BOOST LOG
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
// --- --- sources
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
// --- --- utility
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::severity_logger_mt< ZLog::Level >);
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", ZLog::Level)

ZLog::ZLog(std::string f)
{
    logging::formatter formatter =
        expr::stream
            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
            << expr::smessage;

    if (f.empty())
    {
        logging::add_console_log()->set_formatter(formatter);
    } 
    else
    {
        logging::add_file_log(f)->set_formatter(formatter);
    }


    boost::log::add_common_attributes();
}

void ZLog::log(ZLog::Level level, std::string message)
{
    src::severity_logger_mt< ZLog::Level >& slg = my_logger::get();
    BOOST_LOG_SEV(slg, level) << std::setw(9) << level_str(level) << ": " << message;
}

std::string ZLog::level_str(ZLog::Level level)
{
    std::string s = "UNK";
    switch(level)
    {
        case ZLog::Level::debug:
            s = "debug";
            break;
        case ZLog::Level::info:
            s = "info";
            break;
        case ZLog::Level::warning:
            s = "warning";
            break;
        case ZLog::Level::error:
            s = "error";
            break;
        case ZLog::Level::fatal:
            s = "fatal";
            break;
    }
    return s;
}