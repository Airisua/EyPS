#ifndef EYPS_LOG_H
#define EYPS_LOG_H

#include "Singleton.h"
#include <iostream>
#include <fstream>
#include <string>

#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

class logger : public Singleton<logger>{
    friend class Singleton<logger>;
public:
    using file_sink = boost::log::sinks::asynchronous_sink<boost::log::sinks::text_file_backend>;
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> logger_;
    enum loggerType{
        console = 0,
        file,
    };


    ~logger() = default;
    bool init(const std::string& filename , int type, int level, int maxFileSzie, int maxBackupIndex);

private:
    logger() = default;
};


#define LOG_DEBUG(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(logger::getInst()->logger_, boost::log::trivial::debug) << logEvent;

#define LOG_INFO(logEvent)   BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(logger::getInst()->logger_, boost::log::trivial::info) << logEvent;

#define LOG_WARN(logEvent)   BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(logger::getInst()->logger_, boost::log::trivial::warning) << logEvent;

#define LOG_ERROR(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(logger::getInst()->logger_, boost::log::trivial::error) << logEvent;

#define LOG_FATAL(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(logger::getInst()->logger_, boost::log::trivial::fatal) << logEvent;

#endif //EYPS_LOG_H
