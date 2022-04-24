#include "SceneCamera.hpp"
namespace RabbitEngine
{
    CamLine SceneCamera::FindMark(int beat)
    {
        float bestFitBeat = -1;
        CamLine bestFit = NULL;
        CamLine mark = NULL;
        for (auto mark : Timeline) {
            if (mark.Beat <= beat && bestFitBeat < mark.Beat)
            {
                bestFitBeat = mark.Beat;
                bestFit = mark;
            }
        }
        return bestFit;
    }
    CamLine SceneCamera::FindNextMark(int beat)
    {
        float bestFitBeat = 0xFFFFFF;
        CamLine bestFit = NULL;
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
    void SceneCamera::CreateMark(int beat)
    {
        Timeline.push_back( CamLine(beat));
    }
    bool SceneCamera::HasThisBeat(int beat)
    {
        float bestFitBeat = -1;
        CamLine bestFit = NULL;
        for (auto mark : Timeline) 
        {
            if (mark.Beat == beat)
            {
                return true;
            }
        }
        return false;
    }
}