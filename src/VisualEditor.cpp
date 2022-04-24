#include <math.h>
#include "VisualEditor.hpp"
// Dear ImGui
#include "../imgui/imgui.h"
#include "../ImGui/ImGui_impl_sdl.h"
#include "../ImGui/ImGui_impl_opengl3.h"
namespace RabbitEngine
{
    VisualEditor::VisualEditor(DataHandler dataHandler)
    {
        VisualEditor::_dataHandler = dataHandler;
    }
    void VisualEditor::Draw()
    {
        _time = _dataHandler.GetBeat();
        DrawTimeline();
        if (!_play)
        {
            DrawMark();
        }

        DrawListActors();
        if (_loadFile)
        {
            _dataHandler.LoadJSON("data.json");
            _dataHandler.LoadCameraJSON("camera.json");
        }
        else if (_saveFile)
        {
            _dataHandler.SaveJSON("data.json");
            _dataHandler.SaveCameraJSON("camera.json");
        }
    }
    void VisualEditor::DrawTimeline()
    {
        std::cout << "Preparing to menu Timeline";
        ImGui::Begin("Timeline");
        ImGui::BeginGroup();
        ImVec2 windowPosition = ImGui::GetWindowPos();
        std::cout << "Buttons for Timeline.";
        SceneCamera camera = _dataHandler.Camera();
        if (&camera != nullptr)
        {
            // Note that the timeline is a long list of buttons basically.
            // Drawing only a small range of timeline to the window
            for (int i = (int)_timelineScroll; i < min((float)_timelineLength, _timelineScroll + 32.0f); i++)
            {
                CamLine mark = camera.FindMark(i);
                ImGui::SameLine(-_timelineScroll * _zoom + i * _zoom + 8);
                ImGui::PushItemWidth(80.0f);
                switch (mark.Interpolate)
                {
                case 0:
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
                    break;
                case 1:
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.3f, 0.2f, 1.0f));
                    break;
                case 2:
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.4f, 0.2f, 1.0f));
                    break;
                case 3:
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.4f, 0.2f, 1.0f));
                    break;
                }

                if (mark.Interpolate == 0)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
                }

                if ((int)_time == i)
                {
                    float globalTime = 0.0f; // TODO: Get Global Time
                    float value = (float)(0.5f + sin(globalTime / 100.0f) * 0.5f) * 0.2f;
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(value, value + 0.5f, value, 1.0f));
                }

                bool selected = ImGui::Button(("##{" + to_string(i) + "}").c_str(), ImVec2(_zoom, 32.0f));
                ImVec2 a = ImVec2(-_timelineScroll * _zoom + i * _zoom + 16 + windowPosition.x, windowPosition.y);
                ImVec2 b = ImVec2(-_timelineScroll * _zoom + i * _zoom + 16 + windowPosition.x, windowPosition.y);

                ImDrawList *draw_list = ImGui::GetWindowDrawList();
                draw_list->AddLine(a, b, 0xFFFFFFFF, 1.0f);
                CamLine iterativeMark = camera.FindMark(i);
                if (&iterativeMark != nullptr)
                {
                    if (iterativeMark.Beat == i)
                    {
                        std::cout << "Tick for marker.";
                        ImVec2 pos = ImVec2(
                            -_timelineScroll * _zoom + i * _zoom + 16 + windowPosition.x,
                            28.0f + windowPosition.y);
                        draw_list->AddLine(ImVec2(pos.x, pos.y - 6), ImVec2(pos.x + 6, pos.y), 0xFF00FFFF, 1.0f);
                        draw_list->AddLine(ImVec2(pos.x + 6, pos.y), ImVec2(pos.x, pos.y + 6), 0xFF00FFFF, 1.0f);
                        draw_list->AddLine(ImVec2(pos.x, pos.y + 6), ImVec2(pos.x - 6, pos.y), 0xFF00FFFF, 1.0f);
                        draw_list->AddLine(ImVec2(pos.x - 6, pos.y), ImVec2(pos.x, pos.y - 6), 0xFF00FFFF, 1.0f);
                    }
                }

                draw_list->AddText(ImVec2(-_timelineScroll * _zoom + i * _zoom + 16 + windowPosition.x, 64.0f + windowPosition.y), 0xFFFFFFFF, ("##{" + to_string(i) + "}").c_str());

                if (selected)
                {
                    _time = i;
                    _dataHandler.SetBeat(_time);
                }
            }
        }
        else
        {
            for (int i = (int)_timelineScroll; i < min((float)_timelineLength, _timelineScroll + 32); i++)
            {
                TimelineMark mark = _dataHandler.CurrentItem().FindMark(i);
                ImGui::SameLine(-_timelineScroll * _zoom + i * _zoom + 8);
                ImGui::PushItemWidth(80.0f);
                switch (mark.Interpolate)
                {
                case 0:
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
                    break;
                case 1:
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.3f, 0.2f, 1.0f));
                    break;
                case 2:
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.4f, 0.2f, 1.0f));
                    break;
                case 3:
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.4f, 0.2f, 1.0f));
                    break;
                }

                if (mark.Interpolate == 0)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
                }

                if ((int)_time == i)
                {
                    float globalTime = 0.0f; // TODO: Get Global Time
                    float value = (float)(0.5f + sin(globalTime / 100.0f) * 0.5f) * 0.2f;
                    ImGui::PushStyleColor(ImGuiCol_Button,
                                          ImVec4(value, value + 0.5f, value, 1.0f));
                }
                ImDrawList *draw_list = ImGui::GetWindowDrawList();
                bool selected = ImGui::Button(("##{" + to_string(i) + "}").c_str(), ImVec2(_zoom, 32.0f));
                ImVec2 a = ImVec2(-_timelineScroll * _zoom + i * _zoom + 16 + windowPosition.x, windowPosition.y);
                ImVec2 b = ImVec2(-_timelineScroll * _zoom + i * _zoom + 16 + windowPosition.x, windowPosition.y);
                draw_list->AddLine(a, b, 0xFFFFFFFF, 1.0f);
                TimelineMark iterativeMark = _dataHandler.CurrentItem().FindMark(i);
                if (&iterativeMark != nullptr)
                {
                    if (iterativeMark.Beat == i)
                    {
                        std::cout << "Tick for marker.";
                        ImVec2 pos = ImVec2(-_timelineScroll * _zoom + i * _zoom + 16 + windowPosition.x, 28.0f + windowPosition.y);
                        draw_list->AddLine(ImVec2(pos.x, pos.y - 6), ImVec2(pos.x + 6, pos.y), 0xFF00FFFF, 1.0f);
                        draw_list->AddLine(ImVec2(pos.x + 6, pos.y), ImVec2(pos.x, pos.y + 6), 0xFF00FFFF, 1.0f);
                        draw_list->AddLine(ImVec2(pos.x, pos.y + 6), ImVec2(pos.x - 6, pos.y), 0xFF00FFFF, 1.0f);
                        draw_list->AddLine(ImVec2(pos.x - 6, pos.y), ImVec2(pos.x, pos.y - 6), 0xFF00FFFF, 1.0f);
                    }
                }

                draw_list->AddText(
                    ImVec2(-_timelineScroll * _zoom + i * _zoom + 16 + windowPosition.x,
                           64.0f + windowPosition.y),
                    0xFFFFFFFF, ("##{" + to_string(i) + "}").c_str());

                if (selected)
                {
                    _time = i;
                    _dataHandler.SetBeat(_time);
                }
            }
        }

        ImGui::EndGroup();
        ImGui::NewLine();

        ImGui::PushItemWidth(ImGui::GetWindowWidth());
        std::cout << "Scroller for Timeline.";
        ImGui::SliderFloat("Timeline", &_timelineScroll, 0, VisualEditor::_timelineLength);
        float sliderTime = ImGui::GetWindowWidth() * _time / VisualEditor::_timelineLength;
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->AddLine(ImVec2(8.0f + sliderTime + windowPosition.x, 96.0f + windowPosition.y), ImVec2(8.0f + sliderTime + windowPosition.x, 82.0f + windowPosition.y), 0xFF11FF11, 4.0f);
        ImGui::PushItemWidth(160);
        ImGui::NewLine();
        ImGui::InputInt("Timeline Length", &(VisualEditor::_timelineLength));
        ImGui::NewLine();
        if (!_play)
        {
            if (ImGui::Button("Play", ImVec2(64.0f, 24.0f)))
            {
                _play = true;
                _dataHandler.Play();
            }
        }
        else
        {
            if (ImGui::Button("Pause", ImVec2(64.0f, 24.0f)))
            {
                _play = false;
                _dataHandler.Pause();
            }
        }
        ImGui::End();
        std::cout << "Done menu for Timeline.";
    }
    void VisualEditor::DrawMark()
    {
        cout << "Preparing to menu Marker";
        if (_dataHandler.CameraActive)
        {
            SceneCamera camera = _dataHandler.Camera();
            bool changed = false;
            CamLine mark = camera.FindMark((int)_time);
            int posX = mark.X;
            int posY = mark.Y;
            int interpolate = mark.Interpolate;
            if (mark.Interpolate > 0)
            {
                CamLine nextMark = camera.FindNextMark((int)_time);
                CamLine markInterpolated = CamLine::Lerp(mark, nextMark, _time);
                posX = markInterpolated.X;
                posY = markInterpolated.Y;
            }
            ImGui::Begin("Object");
            ImGui::BeginGroup();
            ImGui::PushItemWidth(32);
            ImGui::SameLine(8);
            changed |= ImGui::InputInt("X", &posX, 0, 0);
            ImGui::SameLine(8 + 64);
            changed |= ImGui::InputInt("Y", &posY, 0, 0);
            ImGui::EndGroup();

            ImGui::PushItemWidth(100);
            bool show = ImGui::BeginCombo("Interpolation Mode", VisualEditor::_interpolationModes[mark.Interpolate].c_str(), ImGuiComboFlags_NoArrowButton);
            if (show)
            {
                int interpolationValue = interpolate;
                if (ImGui::Selectable("None", interpolationValue == 0))
                {
                    interpolate = 0;
                    changed = true;
                }

                if (ImGui::Selectable("Linear", interpolationValue == 1))
                {
                    interpolate = 1;
                    changed = true;
                }

                if (ImGui::Selectable("Smooth", interpolationValue == 2))
                {
                    interpolate = 2;
                    changed = true;
                }

                if (ImGui::Selectable("Ramp", interpolationValue == 3))
                {
                    interpolate = 3;
                    changed = true;
                }
            }
            ImGui::EndCombo();
            ImGui::End();
            if (changed)
            {
                cout << ("Marker changed.");
                bool empty = _dataHandler.CheckForEmpty();
                if (empty)
                {
                    cout << ("Create a new marker");
                    mark = camera.FindMark((int)_time); // we need a new mark since empty has been replaced
                }

                mark.X = posX;
                mark.Y = posY;
                mark.Interpolate = interpolate;
            }
        }
        else
        {
            bool changed = false;
            Actor actor = _dataHandler.CurrentItem();
            TimelineMark mark = actor.FindMark((int)_time);

            int posX = mark.X;
            int posY = mark.Y;
            int posZ = mark.Z;
            int frame = mark.Frame;
            int flipX = mark.FlipX;
            int flipY = mark.FlipY;
            int interpolate = mark.Interpolate;
            if (mark.Interpolate > 0)
            {
                TimelineMark nextMark = actor.FindNextMark((int)_time);
                TimelineMark markInterpolated = TimelineMark::Lerp(mark, nextMark, _time);
                posX = markInterpolated.X;
                posY = markInterpolated.Y;
                posZ = markInterpolated.Z;
                frame = markInterpolated.Frame;
                flipX = markInterpolated.FlipX;
                flipY = markInterpolated.FlipY;
                interpolate = markInterpolated.Interpolate;
            }

            ImGui::Begin("Object");
            ImGui::BeginGroup();
            ImGui::PushItemWidth(32);
            ImGui::SameLine(8);
            changed |= ImGui::InputInt("X", &posX, 0, 0);
            ImGui::SameLine(8 + 64);
            changed |= ImGui::InputInt("Y", &posY, 0, 0);
            ImGui::SameLine(8 + 128);
            changed |= ImGui::InputInt("Z", &posZ, 0, 0);

            ImGui::EndGroup();

            ImGui::PushItemWidth(128);
            ImGui::NewLine();
            ImGui::SameLine(16);
            changed |= ImGui::InputInt("Frame", &frame);
            frame = max(min(frame, _dataHandler.CurrentItem().NumFrames() - 1), 0);

            ImGui::NewLine();
            ImGui::NewLine();
            ImGui::BeginGroup();
            bool flipxb = flipX == 1 ? true : false;
            bool flipyb = flipY == 1 ? true : false;
            ImGui::SameLine(8);
            if (ImGui::RadioButton("FlipX", flipxb))
            {
                if (flipxb)
                {
                    flipX = 0;
                }
                else
                {
                    flipX = 1;
                }

                changed = true;
            }

            ImGui::SameLine(72);
            if (ImGui::RadioButton("FlipY", flipyb))
            {
                if (flipyb)
                {
                    flipY = 0;
                }
                else
                {
                    flipY = 1;
                }

                changed = true;
            }

            ImGui::EndGroup();

            ImGui::NewLine();
            ImGui::PushItemWidth(100);
            bool show = ImGui::BeginCombo("Interpolation Mode", VisualEditor::_interpolationModes[mark.Interpolate].c_str(), ImGuiComboFlags_NoArrowButton);
            if (show)
            {
                int interpolationValue = interpolate;
                if (ImGui::Selectable("None", interpolationValue == 0))
                {
                    interpolate = 0;
                    changed = true;
                }

                if (ImGui::Selectable("Linear", interpolationValue == 1))
                {
                    interpolate = 1;
                    changed = true;
                }

                if (ImGui::Selectable("Smooth", interpolationValue == 2))
                {
                    interpolate = 2;
                    changed = true;
                }

                if (ImGui::Selectable("Ramp", interpolationValue == 3))
                {
                    interpolate = 3;
                    changed = true;
                }
            }

            if (_dataHandler.CurrentItem().HasThisBeat((int)_time) && _time > 0)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, 0xFF111166);
                if (ImGui::Button("Delete"))
                {
                    int index = _dataHandler.CurrentItem().FindIndexWithTheBeat((int)_time);
                    if (index < 0)
                        return;
                    _dataHandler.CurrentItem().RemoveAt(index);
                }

                ImGui::PopStyleColor();
            }

            ImGui::EndCombo();
            ImGui::End();
            if (changed)
            {
                cout << ("Marker changed.");
                bool empty = _dataHandler.CheckForEmpty();
                if (empty)
                {
                    cout << ("Create a new marker");
                    mark = _dataHandler.CurrentItem().FindMark((int)_time); // we need a new mark since empty has been replaced
                }

                mark.X = posX;
                mark.Y = posY;
                mark.Z = posZ;
                mark.Frame = frame;
                mark.FlipX = flipX;
                mark.FlipY = flipY;
                mark.Interpolate = interpolate;
            }

            cout << ("Menu for Marker Done");
        }
    }
    void VisualEditor::DrawListActors()
    {
        cout << ("Actors in the Scene menu");
        ImGui::Begin("Actors in the Scene");
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O"))
            {
                _loadFile = true;
                _saveFile = false;
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                _loadFile = false;
                _saveFile = true;
            }
            ImGui::EndMenu();
        }

        vector<Actor> currentItems = _dataHandler.CurrentItems();
        const char *items[currentItems.size()];
        int i = 0;
        for (Actor item : currentItems)
        {
            items[i++] = item.Name.c_str();
        }
        ImGui::Text("Actors");
        if (ImGui::Button("Camera", ImVec2(112.0f, 20.0f)))
        {
            _dataHandler.CameraActive = true;
        }
        int currentItemIndex = _dataHandler.CurrentItemIndex;

        if (_dataHandler.CameraActive)
        {
            currentItemIndex = -1;
        }
        static int item_current = currentItemIndex;

        bool actorSelected = ImGui::ListBox("##Actors", &item_current, items, currentItems.size(), 12);
        if (actorSelected)
        {
            _dataHandler.CameraActive = false;
        }
        _dataHandler.CurrentItemIndex = currentItemIndex;
        ImGui::End();
        cout << ("Done with Actors in the Scene menu");
    }
}