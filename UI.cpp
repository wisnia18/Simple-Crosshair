#include "UI.h"
#include "Audio.h"
#include "ImageLoader.h"
#include <imgui.h>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

bool UI::s_menuOpen = true;

float UI::s_thickness = 2.0f;
float UI::s_length = 10.0f;
float UI::s_gap = 5.0f;
float UI::s_color[4] = { 0.11f, 0.88f, 0.82f, 1.0f }; 
bool UI::s_dot = false;

bool UI::s_dotSquare = false;
float UI::s_dotSize = 3.0f;
bool UI::s_tShape = false;
float UI::s_outlineThickness = 1.0f;
float UI::s_outlineColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

float UI::s_currentGap = 5.0f;
float UI::s_animSpeed = 15.0f;

bool UI::s_rainbow = false;
float UI::s_rainbowSpeed = 0.5f;
float UI::s_hue = 0.0f;

bool UI::s_breathing = false;
float UI::s_breathingSpeed = 2.0f;
float UI::s_breathingAmplitude = 0.5f;

bool UI::s_spin = false;
float UI::s_spinSpeed = 50.0f;
float UI::s_currentAngle = 0.0f;

bool UI::s_recoil = false;
float UI::s_recoilAmount = 15.0f;
float UI::s_recoilRecovery = 10.0f;

bool UI::s_useCustomImage = false;
char UI::s_imagePath[256] = "assets/custom.png";
void* UI::s_customTexture = nullptr;
int UI::s_customWidth = 0;
int UI::s_customHeight = 0;
float UI::s_customScale = 1.0f;
void* UI::s_presetTextures[4] = {nullptr, nullptr, nullptr, nullptr};

bool UI::s_useCanvasShape = false;
bool UI::s_canvasData[256] = { false };

std::vector<std::string> UI::s_availableSounds;
int UI::s_selectedSoundIndex = 0;

int UI::s_selectedTab = 0;

void UI::RefreshAvailableSounds() {
    s_availableSounds.clear();
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA("assets\\*.*", &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string filename = findData.cFileName;
                if (filename.length() > 4) {
                    std::string ext = filename.substr(filename.length() - 4);
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                    if (ext == ".wav" || ext == ".mp3") {
                        s_availableSounds.push_back(filename);
                    }
                }
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }

    s_selectedSoundIndex = -1;
    std::string currentPath = Audio::s_soundPath;
    std::string currentFile = currentPath.substr(currentPath.find_last_of("/\\") + 1);
    for (int i = 0; i < s_availableSounds.size(); i++) {
        if (s_availableSounds[i] == currentFile) {
            s_selectedSoundIndex = i;
            break;
        }
    }
}

void UI::LoadCustomImage() {
    if (s_customTexture) {
        ((ID3D11ShaderResourceView*)s_customTexture)->Release();
        s_customTexture = nullptr;
    }
    ID3D11ShaderResourceView* srv = nullptr;
    if (ImageLoader::LoadTextureFromFile(s_imagePath, &srv, &s_customWidth, &s_customHeight)) {
        s_customTexture = (void*)srv;
    }
}

void UI::LoadPresetImages() {
    int w, h;
    ID3D11ShaderResourceView* srv;
    if (ImageLoader::LoadTextureFromFile("assets/cross1.png", &srv, &w, &h)) s_presetTextures[0] = (void*)srv;
    if (ImageLoader::LoadTextureFromFile("assets/cross2.png", &srv, &w, &h)) s_presetTextures[1] = (void*)srv;
    if (ImageLoader::LoadTextureFromFile("assets/cross3.png", &srv, &w, &h)) s_presetTextures[2] = (void*)srv;
    if (ImageLoader::LoadTextureFromFile("assets/cross4.png", &srv, &w, &h)) s_presetTextures[3] = (void*)srv;
}

void UI::SetupStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f;

    style.WindowPadding = ImVec2(16.0f, 16.0f);
    style.FramePadding = ImVec2(12.0f, 6.0f);
    style.ItemSpacing = ImVec2(12.0f, 10.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
    style.GrabMinSize = 14.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.05f, 0.07f, 0.95f); 
    colors[ImGuiCol_ChildBg]                = ImVec4(0.09f, 0.08f, 0.10f, 0.40f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.06f, 0.05f, 0.07f, 0.98f);

    colors[ImGuiCol_Border]                 = ImVec4(0.85f, 0.11f, 0.65f, 0.40f); 
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.85f, 0.11f, 0.65f, 0.10f);

    colors[ImGuiCol_FrameBg]                = ImVec4(0.12f, 0.10f, 0.14f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.85f, 0.11f, 0.65f, 0.25f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.85f, 0.11f, 0.65f, 0.45f);

    colors[ImGuiCol_TitleBg]                = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

    colors[ImGuiCol_CheckMark]              = ImVec4(0.95f, 0.15f, 0.70f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.95f, 0.15f, 0.70f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.00f, 0.30f, 0.80f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.85f, 0.11f, 0.65f, 0.60f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.95f, 0.15f, 0.70f, 0.80f);
    colors[ImGuiCol_Border]                 = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_Separator]              = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); 
    colors[ImGuiCol_ButtonActive]           = ImVec4(1.00f, 0.30f, 0.80f, 1.00f);

    colors[ImGuiCol_Header]                 = ImVec4(0.85f, 0.11f, 0.65f, 0.30f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.85f, 0.11f, 0.65f, 0.60f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.95f, 0.15f, 0.70f, 0.90f);

    colors[ImGuiCol_Separator]              = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
}

bool UI::IsMenuOpen() {
    return s_menuOpen;
}

void UI::ToggleMenu() {
    s_menuOpen = !s_menuOpen;
}

void UI::Render() {
    RenderCrosshair();
    if (s_menuOpen) {
        RenderMenu();
    }
}

void UI::RenderCrosshair() {
    ImGuiIO& io = ImGui::GetIO();
    float cx = io.DisplaySize.x * 0.5f;
    float cy = io.DisplaySize.y * 0.5f;

    if (s_rainbow) {
        s_hue += s_rainbowSpeed * io.DeltaTime;
        if (s_hue > 1.0f) s_hue -= 1.0f;
        ImGui::ColorConvertHSVtoRGB(s_hue, 1.0f, 1.0f, s_color[0], s_color[1], s_color[2]);
    }

    if (s_spin) {
        s_currentAngle += s_spinSpeed * io.DeltaTime;
        if (s_currentAngle > 360.0f) s_currentAngle -= 360.0f;
    }

    float currentAlpha = s_color[3];
    if (s_breathing) {
        float breath = (sinf((float)ImGui::GetTime() * s_breathingSpeed) * 0.5f + 0.5f);
        currentAlpha *= (1.0f - s_breathingAmplitude) + (breath * s_breathingAmplitude);
    }

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    if (s_useCustomImage && s_customTexture) {
        float w = s_customWidth * s_customScale;
        float h = s_customHeight * s_customScale;
        ImU32 imgCol = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, currentAlpha));

        if (s_spin) {
            float rad = s_currentAngle * 3.14159f / 180.0f;
            float c = cosf(rad);
            float s = sinf(rad);
            ImVec2 p1 = ImVec2(cx + (-w/2)*c - (-h/2)*s, cy + (-w/2)*s + (-h/2)*c);
            ImVec2 p2 = ImVec2(cx + (w/2)*c - (-h/2)*s, cy + (w/2)*s + (-h/2)*c);
            ImVec2 p3 = ImVec2(cx + (w/2)*c - (h/2)*s, cy + (w/2)*s + (h/2)*c);
            ImVec2 p4 = ImVec2(cx + (-w/2)*c - (h/2)*s, cy + (-w/2)*s + (h/2)*c);
            drawList->AddImageQuad(s_customTexture, p1, p2, p3, p4, ImVec2(0,0), ImVec2(1,0), ImVec2(1,1), ImVec2(0,1), imgCol);
        } else {
            drawList->AddImage(s_customTexture, ImVec2(cx - w/2, cy - h/2), ImVec2(cx + w/2, cy + h/2), ImVec2(0,0), ImVec2(1,1), imgCol);
        }
        return; 
    }

    if (s_useCanvasShape) {
        float cellSize = s_thickness; 
        int grid = 16;
        float totalSize = grid * cellSize;
        float startX = cx - totalSize / 2.0f;
        float startY = cy - totalSize / 2.0f;

        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(s_color[0], s_color[1], s_color[2], currentAlpha));

        if (s_spin) {
            float rad = s_currentAngle * 3.14159f / 180.0f;
            float c = cosf(rad);
            float s = sinf(rad);
            for (int y = 0; y < grid; y++) {
                for (int x = 0; x < grid; x++) {
                    if (s_canvasData[y * grid + x]) {
                        float offsetX = (x * cellSize) - (totalSize / 2.0f) + (cellSize / 2.0f);
                        float offsetY = (y * cellSize) - (totalSize / 2.0f) + (cellSize / 2.0f);

                        float rotX = cx + (offsetX * c - offsetY * s);
                        float rotY = cy + (offsetX * s + offsetY * c);

                        ImVec2 pMin = ImVec2(rotX - cellSize/2, rotY - cellSize/2);
                        ImVec2 pMax = ImVec2(rotX + cellSize/2, rotY + cellSize/2);
                        drawList->AddRectFilled(pMin, pMax, col);
                    }
                }
            }
        } else {
            for (int y = 0; y < grid; y++) {
                for (int x = 0; x < grid; x++) {
                    if (s_canvasData[y * grid + x]) {
                        ImVec2 pMin = ImVec2(startX + x * cellSize, startY + y * cellSize);
                        ImVec2 pMax = ImVec2(pMin.x + cellSize, pMin.y + cellSize);
                        drawList->AddRectFilled(pMin, pMax, col);
                    }
                }
            }
        }
        return;
    }

    float targetGap = s_gap;
    if (s_recoil && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
        targetGap = s_gap + s_recoilAmount;
    }
    s_currentGap += (targetGap - s_currentGap) * (s_recoil ? s_recoilRecovery : s_animSpeed) * io.DeltaTime;

    ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(s_color[0], s_color[1], s_color[2], currentAlpha));
    ImU32 outCol = ImGui::ColorConvertFloat4ToU32(ImVec4(s_outlineColor[0], s_outlineColor[1], s_outlineColor[2], s_outlineColor[3] * currentAlpha));

    auto DrawLineWithOutline = [&](ImVec2 p1, ImVec2 p2) {
        if (s_outlineThickness > 0.0f) {
            drawList->AddLine(p1, p2, outCol, s_thickness + s_outlineThickness * 2.0f);
        }
        drawList->AddLine(p1, p2, col, s_thickness);
    };

    auto RotatePt = [&](float x, float y) -> ImVec2 {
        if (!s_spin) return ImVec2(cx + x, cy + y);
        float rad = s_currentAngle * 3.14159f / 180.0f;
        float s = sinf(rad);
        float c = cosf(rad);
        return ImVec2(cx + x * c - y * s, cy + x * s + y * c);
    };

    if (s_dot) {
        if (s_outlineThickness > 0.0f) {
            if (s_dotSquare)
                drawList->AddRectFilled(ImVec2(cx - s_dotSize - s_outlineThickness, cy - s_dotSize - s_outlineThickness), 
                                      ImVec2(cx + s_dotSize + s_outlineThickness, cy + s_dotSize + s_outlineThickness), outCol);
            else
                drawList->AddCircleFilled(ImVec2(cx, cy), s_dotSize + s_outlineThickness, outCol);
        }

        if (s_dotSquare)
            drawList->AddRectFilled(ImVec2(cx - s_dotSize, cy - s_dotSize), ImVec2(cx + s_dotSize, cy + s_dotSize), col);
        else
            drawList->AddCircleFilled(ImVec2(cx, cy), s_dotSize, col);
    }

    DrawLineWithOutline(RotatePt(-s_currentGap, 0), RotatePt(-s_currentGap - s_length, 0)); 
    DrawLineWithOutline(RotatePt(s_currentGap, 0), RotatePt(s_currentGap + s_length, 0)); 
    DrawLineWithOutline(RotatePt(0, s_currentGap), RotatePt(0, s_currentGap + s_length)); 
    if (!s_tShape) {
        DrawLineWithOutline(RotatePt(0, -s_currentGap), RotatePt(0, -s_currentGap - s_length)); 
    }
}

void UI::RenderMenu() {
    ImGui::SetNextWindowSize(ImVec2(650, 400), ImGuiCond_FirstUseEver);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

    ImGui::Begin("Simple Crosshair", &s_menuOpen, window_flags);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SIMPLE CROSSHAIR");

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 32.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
    if (ImGui::Button("X", ImVec2(24, 24))) {
        s_menuOpen = false;
    }
    ImGui::PopStyleColor();

    ImGui::Spacing();

    ImGui::BeginChild("Sidebar", ImVec2(160, 0), true);

    if (ImGui::Selectable("Design", s_selectedTab == 0)) s_selectedTab = 0;
    if (ImGui::Selectable("Colors", s_selectedTab == 1)) s_selectedTab = 1;
    if (ImGui::Selectable("Animations", s_selectedTab == 2)) s_selectedTab = 2;
    if (ImGui::Selectable("Audio", s_selectedTab == 3)) s_selectedTab = 3;
    if (ImGui::Selectable("Custom PNG", s_selectedTab == 4)) s_selectedTab = 4;
    if (ImGui::Selectable("Pixel Canvas", s_selectedTab == 5)) s_selectedTab = 5;

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Content", ImVec2(0, 0), true);

    ImGui::PushItemWidth(250); 
    if (s_selectedTab == 0) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Shape Settings");

        ImGui::Spacing();
        ImGui::SliderFloat("Length", &s_length, 1.0f, 50.0f);
        ImGui::SliderFloat("Thickness", &s_thickness, 1.0f, 10.0f);
        ImGui::SliderFloat("Gap", &s_gap, 0.0f, 30.0f);
        ImGui::Checkbox("T-Shape (No Top Line)", &s_tShape);
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Outline");
        ImGui::SliderFloat("Outline Thickness", &s_outlineThickness, 0.0f, 5.0f);
        if (s_outlineThickness > 0.0f) {
            ImGui::ColorEdit4("Outline Color", s_outlineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        }
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Center Dot");
        ImGui::Checkbox("Enable Center Dot", &s_dot);
        if (s_dot) {
            ImGui::Checkbox("Square Dot", &s_dotSquare);
            ImGui::SliderFloat("Dot Size", &s_dotSize, 1.0f, 10.0f);
        }
    } else if (s_selectedTab == 1) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Main Color");

        ImGui::Spacing();
        ImGui::ColorPicker4("Color", s_color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaBar);

        ImGui::Spacing();
        ImGui::Checkbox("RGB Rainbow", &s_rainbow);
        if (s_rainbow) {
            ImGui::SliderFloat("Rainbow Speed", &s_rainbowSpeed, 0.1f, 3.0f);
        }
    } else if (s_selectedTab == 2) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Dynamic Animations");

        ImGui::Spacing();

        ImGui::Checkbox("Shooting Recoil", &s_recoil);
        if (s_recoil) {
            ImGui::SliderFloat("Recoil Amount", &s_recoilAmount, 0.0f, 50.0f);
            ImGui::SliderFloat("Recovery Speed", &s_recoilRecovery, 1.0f, 30.0f);
        } else {
            ImGui::SliderFloat("Click Anim Speed", &s_animSpeed, 1.0f, 30.0f);
        }
        ImGui::Spacing();

        ImGui::Checkbox("Breathing (Pulsing Alpha)", &s_breathing);
        if (s_breathing) {
            ImGui::SliderFloat("Breathing Speed", &s_breathingSpeed, 0.1f, 10.0f);
            ImGui::SliderFloat("Breathing Amount", &s_breathingAmplitude, 0.1f, 1.0f);
        }
        ImGui::Spacing();

        ImGui::Checkbox("Spin (Rotation)", &s_spin);
        if (s_spin) {
            ImGui::SliderFloat("Spin Speed", &s_spinSpeed, -360.0f, 360.0f);
        }
    } else if (s_selectedTab == 3) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Audio Settings");

        ImGui::Spacing();

        bool volChanged = ImGui::SliderFloat("Master Volume", &Audio::s_volume, 0.0f, 1.0f);
        if (volChanged) Audio::SetVolume(Audio::s_volume);

        ImGui::Spacing();
        ImGui::Checkbox("Enable Sound Effects", &Audio::s_enableSound);

        if (ImGui::Button("Refresh Sound List")) {
            RefreshAvailableSounds();
        }

        if (!s_availableSounds.empty()) {
            std::vector<const char*> items;
            for (const auto& s : s_availableSounds) {
                items.push_back(s.c_str());
            }

            if (ImGui::Combo("Select Sound", &s_selectedSoundIndex, items.data(), items.size())) {
                if (s_selectedSoundIndex >= 0 && s_selectedSoundIndex < s_availableSounds.size()) {
                    std::string newPath = "assets/" + s_availableSounds[s_selectedSoundIndex];
                    strncpy(Audio::s_soundPath, newPath.c_str(), 256);
                }
            }
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "No .wav or .mp3 files found in assets/!");
        }

        ImGui::Spacing();
        if (ImGui::Button("Test Sound")) Audio::PlaySoundEffect();

    } else if (s_selectedTab == 4) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Custom PNG Image");

        ImGui::Spacing();

        ImGui::TextColored(ImVec4(0.85f, 0.11f, 0.65f, 1.0f), "Presets:");
        for (int i = 0; i < 4; i++) {
            if (s_presetTextures[i]) {
                ImGui::PushID(i);
                std::string btn_id = "Preset_" + std::to_string(i);
                if (ImGui::ImageButton(btn_id.c_str(), s_presetTextures[i], ImVec2(64, 64), ImVec2(0,0), ImVec2(1,1), ImVec4(0.1f, 0.1f, 0.1f, 1.0f), ImVec4(1,1,1,1))) {
                    s_useCustomImage = true;
                    std::string path = "assets/cross" + std::to_string(i + 1) + ".png";
                    strncpy(s_imagePath, path.c_str(), 256);
                    LoadCustomImage();
                }
                ImGui::PopID();
                if ((i + 1) % 4 != 0) ImGui::SameLine();
            }
        }
        ImGui::Spacing();

        ImGui::Checkbox("Use Custom Image Instead of Shape", &s_useCustomImage);
        ImGui::Spacing();
        ImGui::InputText("Image Path", s_imagePath, 256);
        if (ImGui::Button("Load Image")) {
            LoadCustomImage();
        }
        if (s_customTexture) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Loaded successfully: %dx%d", s_customWidth, s_customHeight);
            ImGui::SliderFloat("Image Scale", &s_customScale, 0.1f, 5.0f);
        }

    } else if (s_selectedTab == 5) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Pixel Art Canvas Builder");

        ImGui::Spacing();
        ImGui::Checkbox("Use Canvas Shape", &s_useCanvasShape);
        ImGui::TextDisabled("Draw your own crosshair! Use 'Thickness' in Design tab to scale it.");
        ImGui::Spacing();

        if (ImGui::Button("Clear Canvas")) {
            for (int i = 0; i < 256; i++) s_canvasData[i] = false;
        }

        ImVec2 p = ImGui::GetCursorScreenPos();
        float cellSize = 18.0f; 
        int grid = 16;
        float canvasSize = grid * cellSize;

        ImGui::InvisibleButton("canvas_interact", ImVec2(canvasSize, canvasSize));

        bool isHovered = ImGui::IsItemHovered();

        if (isHovered && (ImGui::IsMouseDown(0) || ImGui::IsMouseDown(1))) {
            ImVec2 mousePos = ImGui::GetMousePos();
            int cellX = (int)((mousePos.x - p.x) / cellSize);
            int cellY = (int)((mousePos.y - p.y) / cellSize);
            if (cellX >= 0 && cellX < grid && cellY >= 0 && cellY < grid) {

                s_canvasData[cellY * grid + cellX] = ImGui::IsMouseDown(0);
            }
        }

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        for (int y = 0; y < grid; y++) {
            for (int x = 0; x < grid; x++) {
                ImVec2 pMin = ImVec2(p.x + x * cellSize, p.y + y * cellSize);
                ImVec2 pMax = ImVec2(pMin.x + cellSize, pMin.y + cellSize);

                int index = y * grid + x;
                bool active = s_canvasData[index];

                ImU32 cellCol = active ? IM_COL32(242, 38, 178, 255) : IM_COL32(30, 30, 35, 255);
                drawList->AddRectFilled(pMin, pMax, cellCol, 0.0f);
                drawList->AddRect(pMin, pMax, IM_COL32(60, 60, 65, 255), 0.0f);
            }
        }

    }
    ImGui::PopItemWidth();

    ImGui::EndChild();

    ImGui::End();
}

void UI::SaveSettings() {
    std::ofstream file("config.txt");
    if (!file.is_open()) return;

    file << "thickness " << s_thickness << "\n";
    file << "length " << s_length << "\n";
    file << "gap " << s_gap << "\n";
    file << "color_r " << s_color[0] << "\n";
    file << "color_g " << s_color[1] << "\n";
    file << "color_b " << s_color[2] << "\n";
    file << "color_a " << s_color[3] << "\n";
    file << "dot " << s_dot << "\n";
    file << "dotSquare " << s_dotSquare << "\n";
    file << "dotSize " << s_dotSize << "\n";
    file << "tShape " << s_tShape << "\n";
    file << "outlineThickness " << s_outlineThickness << "\n";
    file << "rainbow " << s_rainbow << "\n";
    file << "breathing " << s_breathing << "\n";
    file << "spin " << s_spin << "\n";
    file << "recoil " << s_recoil << "\n";
    file << "useCustomImage " << s_useCustomImage << "\n";
    file << "customScale " << s_customScale << "\n";
    file << "imagePath " << std::string(s_imagePath) << "\n";

    file << "useCanvasShape " << s_useCanvasShape << "\n";
    file << "canvasData ";
    for (int i = 0; i < 256; i++) {
        file << (s_canvasData[i] ? "1" : "0");
    }
    file << "\n";

    file << "audioVol " << Audio::s_volume << "\n";
    file << "enableSound " << Audio::s_enableSound << "\n";
    file << "soundPath " << std::string(Audio::s_soundPath) << "\n";
}

void UI::LoadSettings() {
    std::ifstream file("config.txt");
    LoadPresetImages();
    RefreshAvailableSounds();
    if (!file.is_open()) return;

    std::string key;
    while (file >> key) {
        if (key == "thickness") file >> s_thickness;
        else if (key == "length") file >> s_length;
        else if (key == "gap") file >> s_gap;
        else if (key == "color_r") file >> s_color[0];
        else if (key == "color_g") file >> s_color[1];
        else if (key == "color_b") file >> s_color[2];
        else if (key == "color_a") file >> s_color[3];
        else if (key == "dot") file >> s_dot;
        else if (key == "dotSquare") file >> s_dotSquare;
        else if (key == "dotSize") file >> s_dotSize;
        else if (key == "tShape") file >> s_tShape;
        else if (key == "outlineThickness") file >> s_outlineThickness;
        else if (key == "rainbow") file >> s_rainbow;
        else if (key == "breathing") file >> s_breathing;
        else if (key == "spin") file >> s_spin;
        else if (key == "recoil") file >> s_recoil;
        else if (key == "useCustomImage") file >> s_useCustomImage;
        else if (key == "customScale") file >> s_customScale;
        else if (key == "imagePath") file >> s_imagePath;
        else if (key == "useCanvasShape") file >> s_useCanvasShape;
        else if (key == "canvasData") {
            std::string data;
            file >> data;
            for (size_t i = 0; i < data.length() && i < 256; i++) {
                s_canvasData[i] = (data[i] == '1');
            }
        }
        else if (key == "audioVol") {
            file >> Audio::s_volume;
            Audio::SetVolume(Audio::s_volume);
        }
        else if (key == "enableSound") file >> Audio::s_enableSound;
        else if (key == "soundPath") {
            file >> Audio::s_soundPath;
            RefreshAvailableSounds();
        }
    }
}
