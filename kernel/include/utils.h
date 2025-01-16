#pragma once

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define SIZEOF_PTR_ARR(x) sizeof(x) / sizeof(uintptr_t)