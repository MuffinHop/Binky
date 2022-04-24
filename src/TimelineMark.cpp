#include "TimelineMark.hpp"
namespace RabbitEngine {
    TimelineMark::TimelineMark(int x, int y, int z, int frame, int flipx, int flipy, int interpolate, int beat) {
        X = x;
        Y = y;
        Z = z;
        Frame = frame;
        FlipX = flipx;
        FlipY = flipy;
        Interpolate = interpolate;
        Beat = beat;
    }
    TimelineMark::TimelineMark(int beat) {
        Beat = beat;
    }
    float TimelineMark::Lerp(float firstFloat, float secondFloat, float by) {
            return firstFloat * (1 - by) + secondFloat * by;
    }
    TimelineMark TimelineMark::Lerp(TimelineMark a, TimelineMark b, float time) {
        TimelineMark c = TimelineMark((int)time);
        float relative = (float)(time - a.Beat) / (float)(b.Beat - a.Beat);
        c.X = (int)Lerp( (float)a.X, (float)b.X, relative);
        c.Y = (int)Lerp( (float)a.Y, (float)b.Y, relative);
        c.Z = (int)Lerp( (float)a.Z, (float)b.Z, relative);
        c.Frame = (int)Lerp( (float)a.Frame, (float)b.Frame, relative);
        c.FlipX = (int)Lerp( (float)a.FlipX, (float)b.FlipX, relative);
        c.FlipY = (int)Lerp( (float)a.FlipY, (float)b.FlipY, relative);
        return c;
    }
}