#pragma once
#include <exception>
#include <string>
#include <Windows.h> // For HRESULT

class RikiException : public std::exception
{
public:
    RikiException(int line, const char *file) noexcept;
    const char *what() const noexcept override;
    virtual const char *GetType() const noexcept;
    int GetLine() const noexcept;
    const std::string &GetFile() const noexcept;
    std::string GetOriginString() const noexcept;

private:
    int line;
    std::string file;

protected:
    mutable std::string whatBuffer;
};

// ================= GRAPHICS EXCEPTION =================

class GraphicsException : public RikiException
{
public:
    GraphicsException(int line, const char *file, HRESULT hr) noexcept;

    const char *what() const noexcept override;
    const char *GetType() const noexcept override;

    HRESULT GetErrorCode() const noexcept;
    std::string GetErrorString() const noexcept;
    std::string GetErrorDescription() const noexcept;

private:
    HRESULT hr;
};

// ================= HELPER MACRO =================

// Use this for DirectX calls
#define GFX_THROW_FAILED(hrcall)                             \
    {                                                        \
        HRESULT hr;                                          \
        if (FAILED(hr = (hrcall)))                           \
            throw GraphicsException(__LINE__, __FILE__, hr); \
    }
