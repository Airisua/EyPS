#include "log.h"
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

bool logger::init(const std::string& filename, int type, int level, int maxFileSzie, int maxBackupIndex) {
    boost::log::formatter formatter = boost::log::expressions::stream
            <<"["
            << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp",
                                                                                   "%Y-%m-%d %H:%M:%S.%f")
            <<"|"
            << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
            <<"] ["
            << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
            <<"] "
            << boost::log::expressions::smessage
            << "  "
            << boost::log::expressions::format_named_scope("Scope",
                                                           boost::log::keywords::format = "(%c:%l)",
                                                           boost::log::keywords::iteration = boost::log::expressions::reverse,
                                                           boost::log::keywords::depth = 1);


    switch (type) {
        case console: {
            auto consoleSink = boost::log::add_console_log();
            consoleSink->set_formatter(formatter);
            boost::log::core::get()->add_sink(consoleSink);
        }
            break;
        case file: {
            boost::shared_ptr<file_sink > fileSink(new file_sink(
                    boost::log::keywords::file_name = filename,
                    boost::log::keywords::target_file_name = "%Y-%m-%d_%H-%M-%S_%N.log",
                    boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(16, 0, 0),
                    boost::log::keywords::rotation_size = maxFileSzie * 1024 * 1024,
                    boost::log::keywords::open_mode = std::ios::out | std::ios::app
                    ));

            fileSink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
                    boost::log::keywords::target = "../logs",
                    boost::log::keywords::max_size = maxFileSzie * maxBackupIndex * 1024 * 1024,
                    boost::log::keywords::min_free_space = 10 * 1024 * 1024,
                    boost::log::keywords::max_files = 512
                    ));

            fileSink->set_formatter(formatter);
            fileSink->locked_backend()->scan_for_files();
            fileSink->locked_backend()->auto_flush(true);
            boost::log::core::get()->add_sink(fileSink);
        }
            break;
        default: {
            auto consoleSink = boost::log::add_console_log();
            consoleSink->set_formatter(formatter);
            boost::log::core::get()->add_sink(consoleSink);
        }
            break;
    }
    boost::log::add_common_attributes();
    boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= level
    );
    return true;
}