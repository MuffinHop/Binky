#include "CamLine.hpp"
namespace RabbitEngine {
    CamLine::CamLine(int beat)
    {
        Beat = beat;
    }
    float CamLine::Lerp(float firstFloat, float secondFloat, float by)
    {
        return firstFloat * (1 - by) + secondFloat * by;
    }
    CamLine CamLine::Lerp(CamLine a, CamLine b, float time)
    {
        CamLine c = CamLine((int)time);
        float relative = (float)(time - a.Beat) / (float)(b.Beat - a.Beat);
        c.X = (int)Lerp( (float)a.X, (float)b.X, relative);
        c.Y = (int)Lerp( (float)a.Y, (float)b.Y, relative);
        return c;
    }
}