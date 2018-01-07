#pragma once

#include <deque>
#include <mutex>

template <typename T, typename MutexType = std::recursive_mutex>
class AtomicDeque
{
protected:
    std::deque<T> container;
    MutexType mut;

    using LockType = std::lock_guard<MutexType>;
    using Container = std::deque<T>;
public:
    
    typename Container::size_type size() const
    {
        LockType {mut};

        return container.size();
    }

    T pop_return_back()
    {
        LockType {mut};

        T backElem = container.back();
        container.pop_back();
        return backElem;
    }

    template <typename... Args>
    void emplace_front(Args&&... args)
    {
        LockType {mut};
        container.emplace_front(std::forward<Args...>(args...));
    }

    //TODO: specialize for pointer types to just return T instead of T*
    //pass an instance of std::chrono::duration
    //returns nullptr if timed out (in which case no state will be changed)
    template <typename Duration>
    T* pop_return_back_or_wait(Duration timeout)
    {
        LockType {mut};
        //TODO: implement
    }

};
