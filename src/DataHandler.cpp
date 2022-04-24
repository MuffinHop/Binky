#include "DataHandler.hpp"
namespace RabbitEngine
{
    vector<Actor> DataHandler::CurrentItems()
    {
        return _items;
    }
    Actor DataHandler::CurrentItem()
    {
        return _items[CurrentItemIndex];
    }
    SceneCamera DataHandler::Camera()
    {
        return _camera;
    }
    DataHandler::DataHandler()
    {
        _bpm = 125;
        _rowsPerBeat = 4;
        _rowRate = (_bpm / 60.0f) * _rowsPerBeat;
        _camera = SceneCamera();
    }
    DataHandler::DataHandler(int bpm, float rowsPerBeat)
    {
        _bpm = bpm;
        _rowsPerBeat = rowsPerBeat;
        _rowRate = (_bpm / 60.0f) * _rowsPerBeat;
        _camera = SceneCamera();
        LoadJSON("data.json");
        LoadCameraJSON("camera.json");
    }
    float DataHandler::GetBeat()
    {
        float seconds = 0.0f; // TODO: Get time from music;
        return seconds * _rowRate;
    }
    void DataHandler::SetBeat(float beat)
    {
        float seconds = (beat + 1) / _rowRate;
        // TODO: Set time to music.
        // wave.CurrentTime = new TimeSpan(0,0,0,0, (int)(seconds*1000f));
    }
    void DataHandler::Play()
    {
        // TODO: Play Music
    }
    void DataHandler::Pause()
    {
        // TODO: Pause Music
    }
    bool DataHandler::CheckForEmpty()
    {
        if (!_camera.HasThisBeat((int)GetBeat()))
        {
            _camera.CreateMark((int)GetBeat());
            return true;
        }

        return false;
    }
    bool DataHandler::CheckForEmptyCamera()
    {
        if (!_camera.HasThisBeat((int)GetBeat()))
        {
            _camera.CreateMark((int)GetBeat());
            return true;
        }
        return false;
    }
    void DataHandler::LoadJSON(string filename)
    {
        cout << filename << std::endl;
        // TODO: Load JSON for Actors
        std::ifstream inFile;
        inFile.open(filename.c_str()); //open the input file

        std::stringstream strStream;
        strStream << inFile.rdbuf(); //read the file
        std::string str = strStream.str(); //str holds the content of the file
        cout << str << "\n";
        json js = json::parse(str);
        for(int actors = 0; actors<js.size(); actors++) {
            json jsonActor = js[actors];

            string name = jsonActor["Name"];
            int frameHeight = jsonActor["FrameHeight"];
            int frameWidth = jsonActor["FrameWidth"];
            string sourceSpriteSet = jsonActor["SourceSpriteSet"];
            json timelineArray = jsonActor["Timeline"];
            vector<TimelineMark> timelineVectorArray;
            for(int timelines = 0; timelines<timelineArray.size(); timelines++) {
                int x = timelineArray["X"];
                int y = timelineArray["Y"];
                int z = timelineArray["Z"];
                int frame = timelineArray["Frame"];
                int flipX = timelineArray["FlipX"];
                int flipY = timelineArray["FlipY"];
                int interpolate = timelineArray["Interpolate"];
                int beat = timelineArray["Beat"];
                TimelineMark mark = TimelineMark( x, y, z, frame, flipX, flipY, interpolate, beat);
                timelineVectorArray.push_back(mark);
            }
            Actor actor(name, sourceSpriteSet, frameWidth, frameHeight, timelineVectorArray);
            _items.push_back(actor);
        }
    }
    void DataHandler::SaveJSON(string filename)
    {
        // TODO: Save JSON for Actors
    }
    void DataHandler::LoadCameraJSON(string filename)
    {
        // TODO: Load JSON for the Camera
    }
    void DataHandler::SaveCameraJSON(string filename)
    {
        // TODO: Save JSON for the Camera
    }
}