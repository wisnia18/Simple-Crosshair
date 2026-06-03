#pragma once
#include <vector>
#include <string>

class UI {
public:
    static void SetupStyle();
    static void Render();

    static bool IsMenuOpen();
    static void ToggleMenu();

    static void SaveSettings();
    static void LoadSettings();

private:
    static void RenderCrosshair();
    static void RenderMenu();

    static bool s_menuOpen;

    static float s_thickness;
    static float s_length;
    static float s_gap;
    static float s_color[4];
    static bool s_dot;
    static bool s_dotSquare;
    static float s_dotSize;
    static bool s_tShape;
    static float s_outlineThickness;
    static float s_outlineColor[4];

    static float s_currentGap;
    static float s_animSpeed;

    static bool s_rainbow;
    static float s_rainbowSpeed;
    static float s_hue;

    static bool s_breathing;
    static float s_breathingSpeed;
    static float s_breathingAmplitude;

    static bool s_spin;
    static float s_spinSpeed;
    static float s_currentAngle;

    static bool s_recoil;
    static float s_recoilAmount;
    static float s_recoilRecovery;

    static bool s_useCustomImage;
    static char s_imagePath[256];
    static void* s_customTexture; 
    static int s_customWidth;
    static int s_customHeight;
    static float s_customScale;

    static void* s_presetTextures[4];
    static void LoadPresetImages();

    static void LoadCustomImage();

    static bool s_useCanvasShape;
    static bool s_canvasData[256]; 

    static std::vector<std::string> s_availableSounds;
    static int s_selectedSoundIndex;
    static void RefreshAvailableSounds();

    static int s_selectedTab;
};
