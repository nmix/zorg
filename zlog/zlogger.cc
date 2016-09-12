#include "zlogger.h"

// --- BOOST
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

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::logger_mt);

ZLogger::ZLogger() {

    filename = "foo_%N.log";

    logging::add_file_log
    (
        keywords::file_name = filename,
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format =  
        (
            expr::stream
                << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                << std::setw(9) << logging::trivial::severity << ": " 
                << expr::smessage
        )
    );

    boost::log::add_common_attributes();

}


void ZLogger::log(ZLogger::Level level, std::string message) {
    src::logger_mt& lg = my_logger::get();
    BOOST_LOG_SEV(lg, ) << message;
}