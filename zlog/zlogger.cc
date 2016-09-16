#include "zlogger.h"

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

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::severity_logger_mt< ZLogger::Level >);
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", ZLogger::Level)

ZLogger::ZLogger(std::string f)
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

void ZLogger::log(ZLogger::Level level, std::string message)
{
    src::severity_logger_mt< ZLogger::Level >& slg = my_logger::get();
    BOOST_LOG_SEV(slg, level) << std::setw(9) << level_str(level) << ": " << message;
}

std::string ZLogger::level_str(ZLogger::Level level)
{
    std::string s = "UNK";
    switch(level)
    {
        case ZLogger::Level::debug:
            s = "debug";
            break;
        case ZLogger::Level::info:
            s = "info";
            break;
        case ZLogger::Level::warning:
            s = "warning";
            break;
        case ZLogger::Level::error:
            s = "error";
            break;
        case ZLogger::Level::fatal:
            s = "fatal";
            break;
    }
    return s;
}
