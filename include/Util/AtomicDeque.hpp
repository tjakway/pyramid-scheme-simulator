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

    T pop_return_back_st()
    {
        T backElem = container.back();
        container.pop_back();
        return backElem;
    }
public:
    
    typename Container::size_type size() const
    {
        LockType {mut};

        return container.size();
    }

    T pop_return_back()
    {
        LockType {mut};
        return pop_return_back_st();
    }

    template <typename... Args>
    void emplace_front(Args&&... args)
    {
        LockType {mut};
        container.emplace_front(std::forward<Args...>(args...));
    }

    T* pop_return_back_or_null()
    {
        LockType {mut};
        if(container.empty())
        {
            return nullptr;
        }
        else
        {
            //in case our mutex isn't recursive,
            //make sure we don't lock again by calling pop_return_back()
            return pop_return_back_st();
        }
    }
};
