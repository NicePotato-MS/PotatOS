#pragma once

#ifdef __cplusplus
extern "C" {
#endif

inline int isdigit(int chr) { return '0' <= chr && chr <= '9'; }

#ifdef __cplusplus
}
#endif