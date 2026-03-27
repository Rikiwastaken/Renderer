#include "Exceptions.h"
#include <sstream>

RikiException::RikiException(int line, const char *file) noexcept
    : line(line), file(file)
{
}

const char *RikiException::what() const noexcept
{
    // uses a buffer to store the what() string, which is generated on demand
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *RikiException::GetType() const noexcept
{
    return "Exception";
}

int RikiException::GetLine() const noexcept
{
    return line;
}

const std::string &RikiException::GetFile() const noexcept
{
    return file;
}

std::string RikiException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "File: " << file << std::endl
        << "Line: " << line;
    return oss.str();
}
