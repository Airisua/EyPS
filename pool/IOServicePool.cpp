#include "IOServicePool.h"

IOServicePool::IOServicePool(std::size_t size)
        : ioService_(size),nextIOService(0) {
    boost::asio::io_context ctx;
    for(std::size_t i = 0; i < size; ++i) {
        works_.emplace_back(boost::asio::make_work_guard(ioService_[i]));   // 保证没有io事件时run不退出
    }

    // 创建多个线程，每个线程内部启动ioService
    for(auto & i : ioService_) {
        threads_.emplace_back([&i]{
            i.run();
        });
    }
}

boost::asio::io_context &IOServicePool::getIOService() {
    auto& sev = ioService_[nextIOService++];
    if(nextIOService == ioService_.size()) {
        nextIOService = 0;
    }
    return sev;
}

void IOServicePool::stop() {
    for(auto& work_ : works_) {
        work_.reset();
    }

    for(auto& thread_ : threads_) {
        thread_.join();
    }
}
