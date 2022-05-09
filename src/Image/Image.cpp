#include "Image.h"
#include "../stb_image/stb_image_write.h"

namespace rt
{
    Image::Image(uint32_t channels):
        m_Width(0),
        m_Height(0),
        m_Channels(channels)
    {
    }

    Image::Image(uint32_t width, uint32_t height, uint32_t channels):
        Image(channels)
    {    
        resize(width, height);
    }

    bool Image::resize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;

        m_ImageData.resize(m_Width * m_Height * m_Channels, 255);

        return true;
    }

    void Image::setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
    {
        uint32_t index = y * m_Width * m_Channels + x * m_Channels;

        m_ImageData[index + 0] = r;
        m_ImageData[index + 1] = g;
        m_ImageData[index + 2] = b;
    }

    bool Image::write(const std::string& path) const
    {
        stbi_write_png(
            path.c_str(),
            static_cast<int>(m_Width),
            static_cast<int>(m_Height),
            static_cast<int>(m_Channels),
            m_ImageData.data(),
            static_cast<int>(m_Width * m_Channels)
        );

        return true;
    }

    uint8_t* Image::data()
    {
        return m_ImageData.data();
    }

    uint32_t Image::size() const
    {
        return m_Width * m_Height * m_Channels;
    }

    uint32_t Image::width() const
    {
        return m_Width;
    }

    uint32_t Image::height() const
    {
        return m_Height;
    }

    uint32_t Image::channels() const
    {
        return m_Channels;
    }
}