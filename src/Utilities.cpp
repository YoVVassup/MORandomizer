#include "Utilities.h"
#include "LogManager.h"
#include <windows.h>
#include <ctime>
#include <chrono>

std::mt19937 g_Rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

int GetRandomInt(int max) {
    return std::uniform_int_distribution<int>(0, max - 1)(g_Rng);
}

void LogDebug(const char* format, ...) {
    extern bool enableDebugLog;
    if (!enableDebugLog) return;
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    time_t now = time(nullptr);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localtime(&now));
    char formatted[2048];
    snprintf(formatted, sizeof(formatted), "[%s] %s\n", timeStr, buffer);
    LogManager::Instance().Write(formatted);
}

std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

void ParseListBuffer(char* buffer, std::vector<std::string>& listOut, const char* listName) {
    char* p = buffer;
    std::string dumpStr = "";
    while (*p) {
        std::string line(p);
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) line = line.substr(0, commentPos);
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) line = line.substr(0, eqPos);
        line = Trim(line);
        if (!line.empty()) {
            listOut.push_back(line);
            dumpStr += line + " | ";
        }
        p += strlen(p) + 1;
    }
    LogDebug("[System] %s loaded %d items: %s", listName, listOut.size(), dumpStr.c_str());
}

bool EndsWithCaseInsensitive(const char* str, const char* suffix) {
    if (!str || !suffix) return false;
    size_t strLen = strlen(str);
    size_t suffixLen = strlen(suffix);
    if (strLen < suffixLen) return false;
    return _stricmp(str + strLen - suffixLen, suffix) == 0;
}

void PlayBeep(DWORD freq, DWORD duration) {
    extern bool enableBeeps;
    if (enableBeeps) Beep(freq, duration);
}