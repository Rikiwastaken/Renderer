#include "Exceptions.h"
#include <sstream>

RikiException::RikiException(int line, const char *file)
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

// ================= GRAPHICS EXCEPTION =================

GraphicsException::GraphicsException(int line, const char *file, HRESULT hr)
    : RikiException(line, file), hr(hr)
{
}

const char *GraphicsException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << GetErrorString() << ")" << std::endl
        << "[Description] " << GetErrorDescription() << std::endl
        << GetOriginString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *GraphicsException::GetType() const noexcept
{
    return "Graphics Exception";
}

HRESULT GraphicsException::GetErrorCode() const noexcept
{
    return hr;
}

std::string GraphicsException::GetErrorString() const noexcept
{
    switch (hr)
    {
    case S_OK:
        return "S_OK";
    case E_FAIL:
        return "E_FAIL";
    case E_INVALIDARG:
        return "E_INVALIDARG";
    case E_OUTOFMEMORY:
        return "E_OUTOFMEMORY";
    default:
        return "Unknown HRESULT";
    }
}

std::string GraphicsException::GetErrorDescription() const noexcept
{
    char *pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf),
        0, nullptr);

    if (nMsgLen == 0)
    {
        return "Unidentified error code";
    }

    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}
