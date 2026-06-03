# Simple Crosshair

A lightweight, modern, hardware-accelerated crosshair overlay for Windows built with C++, DirectX 11, and ImGui.

## Features

- **High Performance:** Utilizes DirectX 11 for zero-latency, hardware-accelerated rendering.
- **Customization:** Fully customizable crosshairs (length, thickness, gap, color, dot).
- **Pixel Canvas:** Draw your own custom pixel-art crosshairs directly in the app.
- **Audio Effects:** Optional UI click and shoot sounds for better tactile feedback.
- **Custom PNGs:** Load your own image files to use as crosshairs.
- **Animations:** RGB Rainbow and Spin effects.

## Building

This project uses CMake. To build it:

1. Clone the repository.
2. Ensure you have CMake and a C++20 compatible compiler installed (like MSVC).
3. Run the following commands:
   ```bash
   cmake -B build
   cmake --build build --config Release
   ```
4. The executable `SimpleCrosshair.exe` will be located in `build/Release/`.

## Usage

- Press **INSERT** to toggle the configuration menu.
- Changes are automatically saved on exit to `config.txt`.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
