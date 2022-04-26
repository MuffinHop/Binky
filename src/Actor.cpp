#include "Actor.hpp"
#include <stdint.h>
#include <inttypes.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
namespace RabbitEngine
{
    Uint32 getPixel(SDL_Surface *surface, int x, int y)
    {
        int bpp = surface->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to retrieve */
        Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

        switch (bpp)
        {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
                break;

            case 4:
                return *(Uint32 *)p;
                break;

            default:
                return 0;       /* shouldn't happen, but avoids warnings */
        }
    }

    Actor::Actor(string name, string sourceSpriteSet, int frameWidth, int frameHeight, vector<TimelineMark> timeline)
    {
        Name = name;
        SourceSpriteSet = sourceSpriteSet;
        FrameWidth = frameWidth;
        FrameHeight = frameHeight;
        Timeline = timeline;

        #if __GBA__


        #else  // On PC and Mac you have to create 
            int width, height, orig_format;
            int req_format = STBI_rgb_alpha;
            unsigned char* u8data = stbi_load(sourceSpriteSet.c_str(), &width, &height, &orig_format, req_format);
            if (u8data == NULL) {
                SDL_Log("Loading image failed: %s file: %s", stbi_failure_reason(), sourceSpriteSet.c_str());
                exit(1);
            }


            int depth, pitch;
            Uint32 pixel_format;
            if (req_format == STBI_rgb) {
                depth = 24;
                pitch = 3*width; // 3 bytes per pixel * pixels per row
                pixel_format = SDL_PIXELFORMAT_RGB24;
            } else { // STBI_rgb_alpha (RGBA)
                depth = 32;
                pitch = 4*width;
                pixel_format = SDL_PIXELFORMAT_RGBA32;
            }
            uint8_t x8 = 0;
            uint8_t y8 = 0;
            uint8_t width8 = width/8; 
            uint8_t height8 = height/8;
            uint8_t *data = new uint8_t[width * height];
            uint32_t srcIndex = 0;
            uint32_t targetIndex = 0;
            for(int i = 0; i < width8 * height8; i++) {
                int base_x = i % width8;
                int base_y = i / width8;
                for(int y = 0; y < 8; y++) {
                    for(int x = 0; x < 8; x++) {
                        if(depth == 24) {
                            uint8_t red = u8data[srcIndex++];
                            uint8_t green = u8data[srcIndex++];
                            uint8_t blue = u8data[srcIndex++];
                            uint16_t color16 = Bitmap::RGBA( red, green, blue, true);
                            data[targetIndex++] = color16;
                        } else if(depth == 32) {
                            uint8_t red = u8data[srcIndex++];
                            uint8_t green = u8data[srcIndex++];
                            uint8_t blue = u8data[srcIndex++];
                            uint8_t alpha = u8data[srcIndex++];
                            uint16_t color16 = Bitmap::RGBA( red, green, blue, alpha>127);
                            data[targetIndex++] = color16;
                        }
                    }
                }
            }
        #endif // if not __GBA__

        _image = Bitmap( width8, height8, data);
    }
    /**
     * @brief Find's the index of a TimelineMark with a specific beat.
     *
     * @param beat
     * @return int index of a timelinemark
     */
    int Actor::FindIndexWithTheBeat(int beat)
    {
        for (unsigned int i = 0; i < Actor::Timeline.size(); i++)
        {
            if (Actor::Timeline[i].Beat == beat)
            {
                return i;
            }
        }
        return -1;
    }
    TimelineMark Actor::FindTimelineMarkObjectWithBeat(int beat)
    {
        int index = FindIndexWithTheBeat(beat);
        TimelineMark timelineMark = Actor::Timeline[index];
        return timelineMark;
    }
    int Actor::RemoveAt(int index)
    {
        Actor::Timeline.erase(Actor::Timeline.begin() + index);
    }
    Vector4 Actor::GetFrame(int frame)
    {

        int framesPerLine = (int)(_image.RealWidth() / _image.RealHeight());
        int frameX = (int)((frame % framesPerLine) * FrameWidth);
        int frameY = (int)((frame / framesPerLine) * FrameHeight);
        return (Vector4){
            .X = frameX, .Y = frameY, .Z = FrameWidth, .W = FrameHeight};
    }
    int Actor::NumFrames()
    {
        int x = _image.RealWidth() / FrameWidth;
        int y = _image.RealHeight() / FrameHeight;
        return x * y;
    }
    TimelineMark Actor::FindMark(int beat)
    {
        float bestFitBeat = -1;
        TimelineMark bestFit = NULL;
        for (auto mark : Timeline)
        {
            if (mark.Beat <= beat && bestFitBeat < mark.Beat)
            {
                bestFitBeat = mark.Beat;
                bestFit = mark;
            }
        }
        return bestFit;
    }

    TimelineMark Actor::FindNextMark(int beat)
    {
        float bestFitBeat = 0xFFFFFF;
        TimelineMark bestFit = NULL;
        for (auto mark : Timeline)
        {
            if (mark.Beat > beat && bestFitBeat > mark.Beat)
            {
                bestFitBeat = mark.Beat;
                bestFit = mark;
            }
        }
        return bestFit;
    }
    void Actor::CreateMark(int beat)
    {
        Timeline.push_back(TimelineMark(beat));
    }
    bool Actor::HasThisBeat(int beat)
    {
        float bestFitBeat = -1;
        TimelineMark bestFit = NULL;

        for (auto mark : Timeline)
        {
            if (mark.Beat == beat)
            {
                return true;
            }
        }
        return false;
    }
    Bitmap Actor::Source()
    {
        return _image;
    }
}