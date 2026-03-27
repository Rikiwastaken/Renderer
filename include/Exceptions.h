#pragma once
#include <exception>
#include <string>

class RikiException : public std::exception
{
public:
    RikiException(int line, const char *file) noexcept; // constructor
    const char *what() const noexcept override;         // override the what() function from std::exception
    virtual const char *GetType() const noexcept;       // virtual function to get the type of the exception
    int GetLine() const noexcept;                       // function to get the line number where the exception was thrown
    const std::string &GetFile() const noexcept;        // function to get the file name where the exception was thrown
    std::string GetOriginString() const noexcept;       // function to get a string representation of the origin of the exception (file and line number)

private:
    int line;
    std::string file;

protected:
    mutable std::string whatBuffer;
};