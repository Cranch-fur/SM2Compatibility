#include "pch.h"
#include <Windows.h>
#include <thread>
#include <codecvt>
#include <intrin.h>

using namespace std;





wstring GetProcessorName()
{
    int cpuInfo[4] = { -1 };
    char CPUBrandString[0x40];
    wchar_t wideCPUBrandString[0x40];

    memset(CPUBrandString, 0, sizeof(CPUBrandString));

    __cpuid(cpuInfo, 0x80000002);
    memcpy(CPUBrandString, cpuInfo, sizeof(cpuInfo));

    __cpuid(cpuInfo, 0x80000003);
    memcpy(CPUBrandString + 16, cpuInfo, sizeof(cpuInfo));

    __cpuid(cpuInfo, 0x80000004);
    memcpy(CPUBrandString + 32, cpuInfo, sizeof(cpuInfo));

    MultiByteToWideChar(CP_ACP, 0, CPUBrandString, -1, wideCPUBrandString, 0x40);
    if (wideCPUBrandString > 0) 
    {
        return wideCPUBrandString;
    }
    else 
    {
        return L"UNKNOWN PROCESSOR NAME";
    }
}


wstring GetProcessorIdentifier()
{
    wchar_t buffer[1024];
    DWORD bufSize = 1024;
    if (GetEnvironmentVariable(L"PROCESSOR_IDENTIFIER", buffer, bufSize) > 0)
    {
        return buffer;
    }
    else
    {
        return L"UNKNOWN PROCESSOR IDENTIFIER";
    }
}


void Handle_CompatibilityCheck()
{
    if (IsProcessorFeaturePresent(PF_AVX2_INSTRUCTIONS_AVAILABLE) == FALSE)
    {
        wstring processorName = GetProcessorName();
        wstring processorIdentifier = GetProcessorIdentifier();
        wstring message = L"Marvel's Spider-Man 2 requires AVX2 instructions support in order to run!\n\nCPU: " + processorName + L"\n" + processorIdentifier;

        MessageBox(NULL, message.c_str(), L"EXCEPTION_ILLEGAL_INSTRUCTION", MB_OK | MB_ICONERROR);
    }
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            thread(Handle_CompatibilityCheck).detach();
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

