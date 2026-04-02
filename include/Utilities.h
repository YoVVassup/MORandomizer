#pragma once

#include <string>
#include <vector>
#include <random>
#include <windows.h>

// Глобальный генератор случайных чисел (C++11)
extern std::mt19937 g_Rng;

// Функция для получения случайного числа в диапазоне [0, max-1]
int GetRandomInt(int max);

// Логирование в файл (если включено)
void LogDebug(const char* format, ...);

// Вспомогательные строковые функции
std::string Trim(const std::string& str);
void ParseListBuffer(char* buffer, std::vector<std::string>& listOut, const char* listName);
bool EndsWithCaseInsensitive(const char* str, const char* suffix);

// Звуковой сигнал
void PlayBeep(DWORD freq, DWORD duration);