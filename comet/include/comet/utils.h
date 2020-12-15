#pragma once

#include <cstdint>

namespace comet
{
    namespace utils
    {

        static constexpr uint32_t offset = 2166136261u;
        static constexpr uint32_t prime = 16777619u;

        constexpr uint32_t fnv1Impl(uint32_t hash, const char *str) {
            return str[0] == 0 ? hash : fnv1Impl((hash^str[0])*prime, str+1);
        }

        constexpr uint32_t hashStr(const char *input) {
            return fnv1Impl(offset, input);
        }

    } // namespace utils
    
} // namespace comet
