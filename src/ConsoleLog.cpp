#include "ConsoleLog.h"

namespace Mus {
    void ConsolePrint(const char* fmt, ...) {
        auto Console = RE::ConsoleLog::GetSingleton();

        if (!Console) return;

        std::string sfmt = UsePrefix;
        sfmt += fmt;
        fmt = sfmt.c_str();

        std::va_list args;
        va_start(args, fmt);
        Console->VPrint(fmt, args);
        va_end(args);
    }
}
