#ifndef EYPS_SINGLETON_H
#define EYPS_SINGLETON_H

#include <memory>
#include <mutex>

template<typename T>
class Singleton{
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton &operator=(const Singleton<T>&) = delete;
    static std::shared_ptr<T> m_ptr;

public:
    virtual ~Singleton() = default;

    static std::shared_ptr<T> getInst() {
        static std::once_flag flag;
        std::call_once(flag,[&]{
            m_ptr = std::shared_ptr<T>(new T);
        });
        return m_ptr;
    }
};

template<typename T>
std::shared_ptr<T> Singleton<T>::m_ptr = nullptr;

#endif //EYPS_SINGLETON_H
