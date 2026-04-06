#define FULL_WINTARD
#include "Surface.h"
#include <algorithm>

namespace Gdpiplus
{
    using std::max;
    using std::min;
}
#include <gdiplus.h>
#include <sstream>

#pragma comment(lib, "gdiplus.lib")

Surface ::Surface(unsigned int width, unsigned int height) noexcept
    : pBuffer(std::make_unique<Color[]>(width * height)),
      width(width),
      height(height)
{
}

Surface &Surface::operator=(Surface &&donor) noexcept
{
    width = donor.width;
    height = donor.height;
    pBuffer = std::move(donor.pBuffer);
    donor.pBuffer = nullptr;
    return *this;
}

Surface ::Surface(Surface &&source) noexcept
    : pBuffer(std::move(source.pBuffer)),
      width(source.width),
      height(source.height)
{
}

Surface ::~Surface()
{
}

void Surface::Clear(Color fillValue) noexcept
{
    memset(pBuffer.get(), fillValue.dword, width * height * sizeof(Color));
}

void Surface::PutPixel(unsigned int x, unsigned int y, Color c) noexcept
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < width);
    assert(y < height);
    pBuffer[y * width + x] = c;
}

Surface ::Color Surface::GetPixel(unsigned int x, unsigned int y) const noexcept
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < width);
    assert(y < height);
    return pBuffer[y * width + x];
}

unsigned int Surface::GetWidth() const noexcept
{
    return width;
}

unsigned int Surface::GetHeight() const noexcept
{
    return height;
}

Surface::Color *Surface::GetBufferPtr() noexcept
{
    return pBuffer.get();
}

const Surface::Color *Surface::GetBufferPtr() const noexcept
{
    return pBuffer.get();
}

const Surface::Color *Surface::GetBufferPtrConst() const noexcept
{
    return pBuffer.get();
}

Surface Surface::FromFile(const std::string &name)
{
    unsigned int width = 0;
    unsigned int height = 0;

    std::unique_ptr<Color[]> pBuffer;

    {
        // convert filename to wstring
        wchar_t wfilename[512];
        mbstowcs_s(nullptr, wfilename, name.c_str(), _TRUNCATE);

        Gdiplus::Bitmap bitmap(wfilename);
        if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
        {
            std::ostringstream ss;
            ss << "Failed to load image from file: " << name;
            throw Exception(__LINE__, __FILE__, ss.str());
        }

        width = bitmap.GetWidth();
        height = bitmap.GetHeight();
        pBuffer = std::make_unique<Color[]>(width * height);

        for (unsigned int y = 0; y < height; ++y)
        {
            for (unsigned int x = 0; x < width; ++x)
            {
                Gdiplus::Color c;
                bitmap.GetPixel(x, y, &c);
                pBuffer[y * width + x] = Color(c.GetA(), c.GetR(), c.GetG(), c.GetB());
            }
        }
    }
    return Surface(width, height, std::move(pBuffer));
}

void Surface::Save(const std::string &filename) const
{
    auto GetEncoderClsid = [&filename](const WCHAR *format, CLSID *pClsid) -> void
    {
        UINT num = 0;  // number of image encoders
        UINT size = 0; // size of the image encoder array in bytes

        Gdiplus::ImageCodecInfo *pImageCodecInfo = nullptr;

        Gdiplus::GetImageEncodersSize(&num, &size);
        if (size == 0)
        {
            std::ostringstream ss;
            ss << "Failed to get image encoders for saving file: " << filename;
            throw Exception(__LINE__, __FILE__, ss.str());
        }

        pImageCodecInfo = (Gdiplus::ImageCodecInfo *)(malloc(size));
        if (pImageCodecInfo == nullptr)
        {
            std::ostringstream ss;
            ss << "Failed to allocate memory for image encoders when saving file: " << filename;
            throw Exception(__LINE__, __FILE__, ss.str());
        }

        GetImageEncoders(num, size, pImageCodecInfo);

        for (UINT j = 0; j < num; ++j)
        {
            if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
            {
                *pClsid = pImageCodecInfo[j].Clsid;
                free(pImageCodecInfo);
                return;
            }
        }

        free(pImageCodecInfo);
        std::ostringstream ss;
        ss << "saving file: " << filename << " failed because no encoder was found for the specified format";
        throw Exception(__LINE__, __FILE__, ss.str());
    };
    CLSID bmpID; // CLSID for BMP encoder
    GetEncoderClsid(L"image/bmp", &bmpID);

    // conver filename to wstring
    wchar_t wfilename[512];
    mbstowcs_s(nullptr, wfilename, filename.c_str(), _TRUNCATE);

    Gdiplus::Bitmap bitmap(width, height, PixelFormat32bppARGB);
    if (bitmap.Save(wfilename, &bmpID) != Gdiplus::Status::Ok)
    {
        std::ostringstream ss;
        ss << "Failed to save image to file: " << filename;
        throw Exception(__LINE__, __FILE__, ss.str());
    }
}

void Surface::Copy(const Surface &src) noexcept
{
    assert(src.GetWidth() == width);
    assert(src.GetHeight() == height);
    std::copy(src.GetBufferPtrConst(), src.GetBufferPtrConst() + width * height, pBuffer.get());
}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept
    : width(width),
      height(height),
      pBuffer(std::move(pBufferParam))
{
}

// exceptions

Surface::Exception::Exception(int line, const char *file, std::string note) noexcept
    : RikiException(line, file), note(std::move(note))
{
}

const char *Surface::Exception::what() const noexcept
{
    std::ostringstream ss;
    ss << RikiException::what() << std::endl
       << "Note: " << GetNote();
    whatBuffer = ss.str();
    return whatBuffer.c_str();
}

const char *Surface::Exception::GetType() const noexcept
{
    return "Surface Exception";
}

const std::string &Surface::Exception::GetNote() const noexcept
{
    return note;
}