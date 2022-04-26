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
     * @brief Set the Mode 0 object
     *
     * @param bitss only 4 or 8-bit sprites and tiles
     * @return true
     * @return false
     */
    bool Bitmap::SetMode0(uint8_t bits)
    {
        if (bits != 4 || bits != 8)
        {
            return false;
        }
        Bitmap::ChunkyBits = 0;
        Bitmap::TileSpriteBits = bits;
        Bitmap::Mode = 0;
        return true;
    }
    /**
     * @brief Set the Mode 1 object
     *
     * @param bits only 4 or 8-bit sprites and tiles
     * @return true
     * @return false
     */
    bool Bitmap::SetMode1(uint8_t bits)
    {
        if (bits != 4 || bits != 8)
        {
            return false;
        }
        Bitmap::ChunkyBits = 0;
        Bitmap::TileSpriteBits = bits;
        Bitmap::Mode = 1;
        return true;
    }
    /**
     * @brief Set the Mode 2 object, only 8-bits sprites and tiles
     * @return true
     * @return false
     */
    bool Bitmap::SetMode2()
    {
        Bitmap::TileSpriteBits = 8;
        Bitmap::ChunkyBits = 0;
        Bitmap::Mode = 2;
        return true;
    }
    /**
     * @brief Set the Mode 3 - only chunky graphics 8-bits
     *
     * @return true
     * @return false
     */
    bool Bitmap::SetMode3()
    {
        Bitmap::ChunkyBits = 8;
        Bitmap::TileSpriteBits = 0;
        Bitmap::Mode = 3;
        return true;
    }
    /**
     * @brief Set the Mode 4 - only chunky graphics 16-bits
     *
     * @return true
     * @return false
     */
    bool Bitmap::SetMode4()
    {
        Bitmap::ChunkyBits = 16;
        Bitmap::TileSpriteBits = 0;
        Bitmap::Mode = 3;
        return true;
    }
    /**
     * @brief Set the Mode 5 - only chunky graphics 16-bits
     *
     * @return true
     * @return false
     */
    bool Bitmap::SetMode5()
    {
        Bitmap::ChunkyBits = 16;
        Bitmap::TileSpriteBits = 0;
        Bitmap::Mode = 3;
        return true;
    }

    /**
     * @brief Get palette index.
     *
     * @param color 16-bit color value
     * @return palette index, if -1 then it does not exist.
     */
    uint8_t Bitmap::GetPaletteIndex(uint16_t color)
    {
        for (uint8_t i = 0; i < Bitmap::PaletteUsed; i++)
        {
            if (Palette[i] == color)
            {
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
     * @param index 8*8 index
     * @param width8 8 * Width 0..3 -- i.e. 8px to 64px
     * @param height8 8 * Height 0..3 -- i.e. 8px to 64px
     * @param data pixel data, note make sure your bit depth is correct. 32 bytes in 4 bit per cell. 64 bytes in 8bit
     */
    Bitmap::Bitmap(uint8_t width8, uint8_t height8, uint8_t *data)
    {
        uint32_t vram_index;
        uint32_t cell_index;
        _index = Bitmap::UsedCells;
        _width8 = width8;
        _height8 = height8;

        if (Bitmap::TileSpriteBits)
        {
            vram_index = Bitmap::_index * 64;
            for (cell_index = 0; cell_index < 64 * width8 * height8; cell_index++)
            {
                bitmap_memory[vram_index++] = data[cell_index];
            }
        }
        else
        {
            vram_index = Bitmap::_index * 32;
            for (cell_index = 0; cell_index < 32 * width8 * height8; cell_index++)
            {
                bitmap_memory[vram_index++] = data[cell_index];
            }
        }
    }

    /**
     */
    Bitmap::Bitmap()
    {
    }

    uint8_t Bitmap::RealWidth()
    {
        return Bitmap::_width8 * 8;
    }
    uint8_t Bitmap::RealHeight()
    {
        return Bitmap::_height8 * 8;
    }
    uint8_t Bitmap::CellWidth()
    {
        return Bitmap::_width8;
    }
    uint8_t Bitmap::CellHeight()
    {
        return Bitmap::_height8;
    }
    uint32_t Bitmap::VramCell()
    {
        return Bitmap::_index;
    }
}