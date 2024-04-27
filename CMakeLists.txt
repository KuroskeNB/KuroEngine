cmake_minimum_required(VERSION 3.27)

set(PROJECT_NAME KuroEngine)
project(${PROJECT_NAME})

add_subdirectory(KuroEngineCore)
add_subdirectory(KuroEngineEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT KuroEngineEditor)