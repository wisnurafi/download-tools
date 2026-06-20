cmake_minimum_required(VERSION 3.20)
project(DownloadToolsV2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)

add_executable(DownloadToolsV2
    DownloadToolsV2/main.cpp
    DownloadToolsV2/src/Console.cpp
    DownloadToolsV2/src/Input.cpp
    DownloadToolsV2/src/FileManager.cpp
    DownloadToolsV2/src/Downloader.cpp
    DownloadToolsV2/src/Installer.cpp
    DownloadToolsV2/src/Catalog.cpp
    DownloadToolsV2/src/Menu.cpp
)

target_include_directories(DownloadToolsV2 PRIVATE
    DownloadToolsV2
    DownloadToolsV2/src
)

target_link_libraries(DownloadToolsV2 PRIVATE
    CURL::libcurl
    Shell32
    Shlwapi
)

if(MSVC)
    target_compile_options(DownloadToolsV2 PRIVATE /bigobj /utf-8)
endif()

# Copy the runtime catalog next to the binary.
add_custom_command(TARGET DownloadToolsV2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/DownloadToolsV2/software_catalog.json"
        "$<TARGET_FILE_DIR:DownloadToolsV2>"
)
