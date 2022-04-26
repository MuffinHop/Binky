#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#ifdef __GBA__
#define GBA
#endif

// BGR 15-bit always
#ifdef MEGADRIVE
static uint16_t Palette[4 * 16];
#define MAX_PALETTE 64
#elseif __GBA__
/**
 * 05000000-050001FF - BG Palette RAM (512 bytes, 256 colors)
 * 05000200-050003FF - OBJ Palette RAM (512 bytes, 256 colors)
 **/
static uint16_t *Palette = (uint16_t *)0x06000000;
#define MAX_PALETTE 512
#else
static uint16_t Palette[1024];
#define MAX_PALETTE 512
#endif
#ifdef __GBA__
/**
 * 06000000-06017FFF on GBA
 **/
static uint16_t *bitmap_memory = (uint16_t *)0x06000000;
#else
static uint16_t bitmap_memory[0x17FFF];
#endif
namespace RabbitEngine
{
        class Bitmap
        {
        private:
                /**
                 * Both Mega Drive and GBA store their color data in BRG.
                 * ! Mega Drive format:
                 * *FEDCBA9876543210
                 * *....BBB.GGG.RRR.
                 *
                 * ? Sprite
                 * Can be 8x8, 16x16, 24x24 and 32x32 pixels tall
                 *
                 * ! Game Boy Advanced format:
                 * *FEDCBA9876543210
                 * *aBBBBBGGGGGRRRRR
                 * highest bit is transparency.
                 *
                 * ? Sprites
                 * 8x8, 16x8, 32x8, 64x64... many such variations 4-bits used for this information
                 *
                 * https://rust-console.github.io/gbatek-gbaonly/#lcdvramcharacterdata
                 *
                 * ? Mode 3
                 * Just 16bit color chunky bitmap, no backbuffer. Cuts into sprite data.
                 *
                 * ? Mode 4
                 * Similar to DOS 13h, yes backbuffer.
                 *
                 * ? Mode 5
                 * Just like Mode 3, but has a backbuffer. Lower resolution. 160x128.
                 *
                 * ? Mode 0
                 * All 4 background layers are available.
                 * Tiles may have 4-bit or 8-bit depth.
                 * Minimum map size is 32x32, maximum is 64x64.
                 *
                 * ? Mode 1
                 * Three background layers, BG 2 can rotate and scale. BG 0 and 1 are non-scrolling.
                 * Tiles may have 4-bit or 8-bit depth.
                 * Minimum map size is 32x32, maximum is 64x64.
                 *
                 * ? Mode 2
                 * Two layers, 2 and 3 are both for scale/rotate variety.
                 * 8-bit only
                 *
                 * ? Cell
                 * 8x8
                 * ? GBA 4-bit
                 * 32 bytes per cell
                 * ? GBA 8-bit
                 * 64 bytes per cell
                 */
                uint8_t _width8, _height8;
                uint32_t _index;
                uint8_t *_data;

        public:
                static int Mode;
                static uint8_t TileSpriteBits;
                static uint8_t ChunkyBits; // also known as Bitmap graphics.
                static uint16_t UsedCells;
                /**
                 * @brief How many of the 256 palette is used.
                 */
                static int PaletteUsed;

                /**
                 * @brief Set the Mode 0 object
                 *
                 * @param bitss only 4 or 8-bit sprites and tiles
                 * @return true
                 * @return false
                 */
                static bool SetMode0(uint8_t bits);
                /**
                 * @brief Set the Mode 1 object
                 *
                 * @param bits only 4 or 8-bit sprites and tiles
                 * @return true
                 * @return false
                 */
                static bool SetMode1(uint8_t bits);
                /**
                 * @brief Set the Mode 2 object, only 8-bits sprites and tiles
                 * @return true
                 * @return false
                 */
                static bool SetMode2();
                /**
                 * @brief Set the Mode 3 - only chunky graphics 8-bits
                 *
                 * @return true
                 * @return false
                 */
                static bool SetMode3();
                /**
                 * @brief Set the Mode 4 - only chunky graphics 16-bits
                 *
                 * @return true
                 * @return false
                 */
                static bool SetMode4();
                /**
                 * @brief Set the Mode 5 - only chunky graphics 16-bits
                 *
                 * @return true
                 * @return false
                 */
                static bool SetMode5();

                /**
                 * @brief Adds color to palette memory
                 *
                 * @param color 16-bit color value
                 * @param index
                 * @return true success
                 * @return false no success
                 */
                static bool AddToPalette(uint16_t color);

                /**
                 * @brief Get palette index.
                 *
                 * @param color 16-bit color value
                 * @return palette index, if -1 then it does not exist.
                 */
                static uint8_t GetPaletteIndex(uint16_t color);

                /**
                 * @brief Assumes 5-bits each color component. On devices with less bits, depth is lost.
                 *
                 * @param R
                 * @param G
                 * @param B
                 * @return uint16_t
                 */
                static uint16_t RGBA(uint8_t R, uint8_t G, uint8_t B, bool transparent);

                /**
                 * @brief Push your new bitmap in 8x8
                 *
                 * @param width8 8 * Width
                 * @param height8 8 * Height
                 * @param data pixel data, note make sure your bit depth is correct. 32 bytes in 4 bit per cell. 64 bytes in 8bit
                 */
                Bitmap(uint8_t width8, uint8_t height8, uint8_t *data);

                Bitmap();

                uint8_t RealWidth();
                uint8_t RealHeight();
                uint8_t CellWidth();
                uint8_t CellHeight();
                uint32_t VramCell();
        };
}
