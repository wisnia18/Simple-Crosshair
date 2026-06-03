#pragma once
#include <d3d11.h>

class ImageLoader {
public:
    static void Initialize(ID3D11Device* device);
    static bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

private:
    static ID3D11Device* s_device;
};
