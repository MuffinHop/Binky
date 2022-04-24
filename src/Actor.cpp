#include "Actor.hpp"
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
        SDL_Surface* sdlImage = IMG_Load(sourceSpriteSet.c_str());
        if (sdlImage == NULL) {
            std::cout << "Error loading image: " << IMG_GetError();
        }
        uint8_t x8 = 0;
        uint8_t y8 = 0;
        uint8_t width8 = sdlImage.w/8; 
        uint8_t height8 = sdlImage.h/8;
        uint8_t data[width*height];
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                Uint32 color = getPixel(sdlImage, x, y);
                uint16_t color16 = Bitmap::RGBA(color>>24, (color>>16) & 0xFF, (color>>8) & 0xFF, (color&0xFF)<127);
                
            }
        }
        _image = new Bitmap(uint8_t x8, uint8_t y8, uint8_t width8, uint8_t height8, uint8_t *data);
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