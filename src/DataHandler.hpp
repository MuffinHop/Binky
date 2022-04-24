#include "Actor.hpp"
#include "SceneCamera.hpp"
#include <cstdlib>
#include <iostream>
#include <list>
#include <iterator>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
using namespace std;
using namespace nlohmann;
namespace RabbitEngine {
    class DataHandler {
        private:
            vector<Actor> _items;
            SceneCamera _camera;
            float _bpm;
            float _rowsPerBeat;
            float _rowRate;

        public:
            int CurrentItemIndex;
            bool CameraActive;
            
            vector<Actor> CurrentItems();
            Actor CurrentItem();
            SceneCamera Camera();
            DataHandler();
            DataHandler(int bpm, float rowsPerBeat);
            float GetBeat();
            void SetBeat(float beat);
            void Play();
            void Pause();
            bool CheckForEmpty();
            bool CheckForEmptyCamera();
            void LoadJSON(string filename);
            void SaveJSON(string filename);
            void LoadCameraJSON(string filename);
            void SaveCameraJSON(string filename);
    };
}