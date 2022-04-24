#include "Bitmap.hpp"
namespace RabbitEngine
{
    bool Bitmap::AddToPalette(uint16_t color)
    {
        if (Bitmap::PaletteUsed >= MAX_PALETTE)
            return false;

        Palette[Bitmap::PaletteUsed++] = color;

        return true;
    }

    /**
     * @brief Get palette index.
     *
     * @param color 16-bit color value
     * @return palette index, if -1 then it does not exist.
     */
    static uint8_t GetPaletteIndex(uint16_t color) {
        for (uint8_t i = 0; i < Bitmap::PaletteUsed; i++)  {
            if(Palette[i] == color) {
                return i;
            }
        }
        return -1;
    }

    /**
     * @brief Assumes 5-bits each color component. On devices with less bits, depth is lost.
     *
     * @param R
     * @param G
     * @param B
     * @return uint16_t
     */
    uint16_t Bitmap::RGBA(uint8_t R, uint8_t G, uint8_t B, bool transparent)
    {
#ifdef GBA
        return (R & 0x1F) | (G & 0x1F) << 5 | (B & 0x1F) << 10;
#else
        // TODO: Add support for MD
#endif
    }

    /**
     * @brief Push your new bitmap in 8x8
     *
     * @param x8 8 * X coordinate
     * @param y8 8 * Y coordinate
     * @param width8 8 * Width 0..3 -- i.e. 8px to 64px
     * @param height8 8 * Height 0..3 -- i.e. 8px to 64px
     * @param data pixel data, note make sure your bit depth is correct. 32 bytes in 4 bit per cell. 64 bytes in 8bit
     */
    Bitmap::Bitmap(uint8_t x8, uint8_t y8, uint8_t width8, uint8_t height8, uint8_t *data)
    {
        uint32_t vram_index;
        uint32_t cell_index;
        _x8 = x8;
        _y8 = y8;
        _width8 = width8;
        _height8 = height8;

        if (Bitmap::Mode_8bit)
        {
            vram_index = (x8 + y8 * 16) * 64;
            for (cell_index = 0; cell_index < 64 * width8 * height8; cell_index++)
            {
                bitmap_memory[vram_index++] = data[cell_index];
            }
        }
        else
        {
            vram_index = (x8 + y8 * 32) * 32;
            for (cell_index = 0; cell_index < 32 * width8 * height8; cell_index++)
            {
                bitmap_memory[vram_index++] = data[cell_index];
            }
        }
    }

    uint8_t Bitmap::RealWidth()
    {
        return _width8 * 8;
    }
    uint8_t Bitmap::RealHeight()
    {
        return _height8 * 8;
    }
    uint8_t Bitmap::CellWidth()
    {
        return _width8;
    }
    uint8_t Bitmap::CellHeight()
    {
        return _height8;
    }
    uint8_t Bitmap::VramCellLocationX()
    {
        return _x8;
    }
    uint8_t Bitmap::VramCellLocationY()
    {
        return _y8;
    }
}