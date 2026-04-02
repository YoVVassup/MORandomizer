#pragma once

#include <string>
#include <vector>
#include <random>
#include <windows.h>
#include <thread>
#include <chrono>

// Global Random Number Generator (C++11)
extern std::mt19937 g_Rng;

// Function to get a random number in the range [0, max-1]
int GetRandomInt(int max);

// Logging to a file (uses LogManager)
void LogDebug(const char* format, ...);

// Helper String Functions
std::string Trim(std::string_view str) noexcept;
void ParseListBuffer(char* buffer, std::vector<std::string>& listOut, const char* listName);
bool EndsWithCaseInsensitive(const char* str, const char* suffix) noexcept;

// Bleeper
void PlayBeep(DWORD freq, DWORD duration);

// Delay in milliseconds (uses std::this_thread::sleep_for)
inline void SleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}