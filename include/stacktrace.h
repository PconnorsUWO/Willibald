#pragma once

#include <iostream>
#include <csignal>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

namespace StackTrace
{
    inline void printStackTrace()
    {
        const int maxFrames = 100;
        void* stack[maxFrames];
        unsigned short frames = CaptureStackBackTrace(0, maxFrames, stack, nullptr);

        HANDLE process = GetCurrentProcess();
        // Initialize symbol handler for the process.
        if (!SymInitialize(process, NULL, TRUE))
        {
            std::cerr << "Failed to initialize symbol handler." << std::endl;
            return;
        }

        SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        if (!symbol)
        {
            std::cerr << "Failed to allocate memory for symbol info." << std::endl;
            return;
        }
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        for (unsigned int i = 0; i < frames; i++)
        {
            DWORD64 address = reinterpret_cast<DWORD64>(stack[i]);
            if (SymFromAddr(process, address, 0, symbol))
            {
                std::cerr << frames - i - 1 << ": " << symbol->Name
                          << " - 0x" << std::hex << symbol->Address << std::dec << std::endl;
            }
            else
            {
                std::cerr << frames - i - 1 << ": " << "???"
                          << " - 0x" << std::hex << address << std::dec << std::endl;
            }
        }

        free(symbol);
        SymCleanup(process);
    }

    inline void seTranslator(unsigned int code, EXCEPTION_POINTERS* info)
    {
        std::cerr << "SEH exception caught: code " << code << std::endl;
        printStackTrace();
        exit(EXIT_FAILURE);
    }

    inline void signalHandler(int signum)
    {
        std::cerr << "Error: signal " << signum << std::endl;
        printStackTrace();
        exit(EXIT_FAILURE);
    }
    
    inline void registerSignalHandler()
    {
        _set_se_translator(seTranslator);
        signal(SIGSEGV, signalHandler);
    }
}

#else  // macOS/Linux

#include <execinfo.h>

namespace StackTrace
{
    inline void printStackTrace()
    {
        const int maxFrames = 100;
        void* stack[maxFrames];
        int frames = backtrace(stack, maxFrames);
        char** symbols = backtrace_symbols(stack, frames);

        if (!symbols)
        {
            std::cerr << "Failed to get stack trace symbols." << std::endl;
            return;
        }

        for (int i = 0; i < frames; ++i)
        {
            std::cerr << frames - i - 1 << ": " << symbols[i] << std::endl;
        }

        free(symbols);
    }

    inline void signalHandler(int signum)
    {
        std::cerr << "Error: signal " << signum << std::endl;
        printStackTrace();
        exit(EXIT_FAILURE);
    }

    inline void registerSignalHandler()
    {
        signal(SIGSEGV, signalHandler);
    }
}

#endif  // _WIN32

