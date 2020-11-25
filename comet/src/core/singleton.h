#pragma once

namespace comet
{
    
    template<typename T>
    class Singleton
    {
    public:
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        ~Singleton() {};

        static T& getInstance()
        {
            static T s_instance{};
            return s_instance;
        }
    
    protected:
        Singleton() {};
    };

} // namespace comet
