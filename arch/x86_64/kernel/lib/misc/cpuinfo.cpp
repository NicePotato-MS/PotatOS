#include <stdint.h>
#include <stdbool.h>
#include <cpuid.h>

#include <cpuinfo.h>

static unsigned int eax, ebx, ecx, edx, unused;

static char model_string[49];

char* cpuinfo::GetModelString() {
    __get_cpuid(0x80000002, &eax, &ebx, &ecx, &edx);
    *((unsigned int*)model_string) = eax;
    *((unsigned int*)(model_string + 4)) = ebx;
    *((unsigned int*)(model_string + 8)) = ecx;
    *((unsigned int*)(model_string + 12)) = edx;

    __get_cpuid(0x80000003, &eax, &ebx, &ecx, &edx);
    *((unsigned int*)(model_string + 16)) = eax;
    *((unsigned int*)(model_string + 20)) = ebx;
    *((unsigned int*)(model_string + 24)) = ecx;
    *((unsigned int*)(model_string + 28)) = edx;

    __get_cpuid(0x80000004, &eax, &ebx, &ecx, &edx);
    *((unsigned int*)(model_string + 32)) = eax;
    *((unsigned int*)(model_string + 36)) = ebx;
    *((unsigned int*)(model_string + 40)) = ecx;
    *((unsigned int*)(model_string + 44)) = edx;

    model_string[48] = '\0';
    return model_string;
}

bool cpuinfo::HasAPIC() {
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_APIC;
}