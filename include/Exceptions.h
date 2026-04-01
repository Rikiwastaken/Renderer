#pragma once
#include <exception>
#include <string>
#include <Windows.h> // For HRESULT

class RikiException : public std::exception
{
public:
    RikiException(int line, const char *file);
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
