#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>

class WindowsMessageMap
{
public:
    WindowsMessageMap();

    // Returns a formatted string describing the message, LPARAM, and WPARAM
    std::string operator()(DWORD msg, LPARAM lp = 0, WPARAM wp = 0) const;

private:
    // Map of Windows message codes to their string names
    std::unordered_map<DWORD, std::string> map;
};