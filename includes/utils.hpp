#pragma once

#include <print>

#define error(fmt, ...)                                                   \
    std::print (fmt "\n", ##__VA_ARGS__);                                 \
    exit (1);
