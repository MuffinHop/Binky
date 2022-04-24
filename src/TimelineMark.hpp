#include "Vector4.hpp"
namespace RabbitEngine {
    class TimelineMark {
        public:
            int X, Y, Z;
            int Frame;
            int FlipX, FlipY;
            int Interpolate;
            int Beat;

            TimelineMark(int x, int y, int z, int frame, int flipx, int flipy, int interpolate, int beat);
            TimelineMark(int beat);
            static TimelineMark Lerp(TimelineMark a, TimelineMark b, float time);
        private:
            static float Lerp(float firstFloat, float secondFloat, float by);

     };
}