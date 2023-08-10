#ifndef EYPS_IOSERVICEPOOL_H
#define EYPS_IOSERVICEPOOL_H

#include "../util/Singleton.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <memory>
#include <thread>
#include <vector>

class IOServicePool : public Singleton<IOServicePool>{
    friend class Singleton<IOServicePool>;
public:
    using IOService = boost::asio::io_context;
//    using Work = boost::asio::io_context::work; // 保证没有任务的时候run不会退出
    using Work = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
//    using WorkPtr = std::unique_ptr<Work>;
    ~IOServicePool()  override = default;
    IOServicePool(const IOServicePool&) = delete;
    IOServicePool& operator=(const IOServicePool&) = delete;
    boost::asio::io_context& getIOService();
    void stop();

private:
    IOServicePool(std::size_t size = std::thread::hardware_concurrency());
    std::vector<IOService> ioService_;
    std::vector<Work> works_;
    std::vector<std::thread> threads_;
    std::size_t nextIOService;
};


#endif //EYPS_IOSERVICEPOOL_H
