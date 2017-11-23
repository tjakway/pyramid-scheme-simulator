#pragma once

#include <mutex>

class Lockable 
{
protected:
    std::mutex mutex;
};
