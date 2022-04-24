#include "CamLine.hpp"
#include <cstdlib>
#include <iostream>
#include <list>
using namespace std;
namespace RabbitEngine
{
    class SceneCamera {
        public:
            int Width;
            int Height;
            list<CamLine> Timeline;

            CamLine FindMark(int beat);
            CamLine FindNextMark(int beat);
            void CreateMark(int beat);
            bool HasThisBeat(int beat);
    };
} // namespace SceneCamera
