#pragma once

#include <vector>
#include <string>

namespace rt
{
    class Image
    {
    public:
        Image(uint32_t channels = 3);
        Image(uint32_t width, uint32_t height, uint32_t channels = 3);

        bool resize(uint32_t width, uint32_t height);
        void setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
        bool write(const std::string& path) const;

        uint8_t* data();
        uint32_t size() const;
        uint32_t width() const;
        uint32_t height() const;
        uint32_t channels() const;

    private:
        uint32_t m_Width;
        uint32_t m_Height;
        uint32_t m_Channels;
        std::vector<uint8_t> m_ImageData;
    };
}