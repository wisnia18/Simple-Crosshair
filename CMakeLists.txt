cmake_minimum_required(VERSION 3.15)
project(SimpleCrosshair)

set(CMAKE_CXX_STANDARD 20)

# Set the ImGui directory
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

# Include ImGui sources and the specific backends for Win32 and DirectX 11
file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
)

add_executable(${PROJECT_NAME} WIN32
    main.cpp
    OverlayWindow.cpp
    UI.cpp
    Audio.cpp
    ImageLoader.cpp
    ${IMGUI_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Link Windows and DirectX libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    d3d11
    d3dcompiler
    dxgi
    dwmapi
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
