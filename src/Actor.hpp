#include "TimelineMark.hpp"
#include "Bitmap.hpp"
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <cstdint>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;
namespace RabbitEngine
{
    class Actor
    {
    public:
        string Name;
        string SourceSpriteSet;
        int FrameWidth;
        int FrameHeight;
        vector<TimelineMark> Timeline;
        
        Actor(string name, string sourceSpriteSet, int frameWidth, int frameHeight, vector<TimelineMark> timeline);
        Vector4 GetFrame(int frame);
        int NumFrames();
        TimelineMark FindMark(int beat);
        TimelineMark FindNextMark(int beat);
        void CreateMark(int beat);
        bool HasThisBeat(int beat);
        Bitmap Source();
        int FindIndexWithTheBeat(int beat);
        TimelineMark FindTimelineMarkObjectWithBeat(int beat);
        int RemoveAt(int index);

    private:
        Bitmap _image;
    };
}