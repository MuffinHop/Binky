#include <string>
#include "DataHandler.hpp"
namespace RabbitEngine
{
    class VisualEditor
    {
    private:
        /**
         * @brief horizontal scroller for timeline view
         **/
        float _timelineScroll;
        /**
         * @brief length of the scroller
         **/
        int _timelineLength = 100;
        /**
         * @brief timeline scroller zoom.
         **/
        int _zoom;
        bool _play;
        float _time;
        bool _loadFile;
        bool _saveFile;
        std::string _interpolationModes[4] = {
            "None", "Linear", "Smooth", "Ramp"};
        DataHandler _dataHandler;
        void DrawTimeline();
        void DrawMark();
        void DrawListActors();
        int FindIndexWithTheBeat(int beat);
        int RemoveAt(int index);

    public:
        VisualEditor(DataHandler dataHandler);
        void Draw();
    };
}