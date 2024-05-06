#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

void halt(void);
void panic(unsigned int exitcode);

#ifdef __cplusplus
}
#endif