#pragma once

#include <cstdio>
#include <string>
#include <mutex>

class LogManager {
public:
    static LogManager& Instance() {
        static LogManager instance;
        return instance;
    }

    void Open(const std::string& filePath) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (m_File) fclose(m_File);
        fopen_s(&m_File, filePath.c_str(), "a");
    }

    void Close() {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (m_File) {
            fclose(m_File);
            m_File = nullptr;
        }
    }

    void Write(const char* format, ...) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (!m_File) return;
        va_list args;
        va_start(args, format);
        vfprintf(m_File, format, args);
        va_end(args);
        fflush(m_File);
    }

    ~LogManager() { Close(); }

private:
    LogManager() : m_File(nullptr) {}
    FILE* m_File;
    std::mutex m_Mutex;
};