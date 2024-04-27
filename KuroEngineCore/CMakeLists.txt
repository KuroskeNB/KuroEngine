cmake_minimum_required(VERSION 3.27)

set(ENGINE_PROJECT_NAME KuroEngineCore)

set(ENGINE_PUBLIC_INCLUDES
	includes/KuroEngineCore/Application.h
        includes/KuroEngineCore/Log.h
        includes/KuroEngineCore/Events.h
        includes/KuroEngineCore/Camera.h
        includes/KuroEngineCore/Inputs.h
)

set(ENGINE_PRIVATE_INCLUDES
    src/KuroEngineCore/Window.h
    src/KuroEngineCore/Interface/UserInterface.h
    src/KuroEngineCore/Render/OpenGL/ShaderProgram.h
    src/KuroEngineCore/Render/OpenGL/VertexBuffer.h
    src/KuroEngineCore/Render/OpenGL/VertexArray.h
    src/KuroEngineCore/Render/OpenGL/IndexBuffer.h
    src/KuroEngineCore/Render/OpenGL/Rendering_OpenGl.h
    src/KuroEngineCore/Render/OpenGL/Texture2D.h
)

set(ENGINE_PRIVATE_SOURCES
	src/KuroEngineCore/Application.cpp
        src/KuroEngineCore/Log.cpp
        src/KuroEngineCore/Window.cpp
        src/KuroEngineCore/Inputs.cpp
        src/KuroEngineCore/Events.cpp
        src/KuroEngineCore/Camera.cpp
        src/KuroEngineCore/Render/OpenGL/ShaderProgram.cpp
        src/KuroEngineCore/Render/OpenGL/VertexBuffer.cpp
        src/KuroEngineCore/Render/OpenGL/VertexArray.cpp
        src/KuroEngineCore/Render/OpenGL/IndexBuffer.cpp
        src/KuroEngineCore/Render/OpenGL/Rendering_OpenGl.cpp
        src/KuroEngineCore/Render/OpenGL/Texture2D.cpp
        src/KuroEngineCore/Interface/UserInterface.cpp
)

add_library(${ENGINE_PROJECT_NAME} STATIC
 ${ENGINE_PUBLIC_INCLUDES}
${ENGINE_PRIVATE_INCLUDES}
${ENGINE_PRIVATE_SOURCES}
 
)
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_20)


include_directories(${CMAKE_SOURCE_DIR}/../externals/stb)
link_directories(${CMAKE_SOURCE_DIR}/../externals/stb) 

add_subdirectory(../externals/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../externals/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../externals/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glm)

add_subdirectory(../externals/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

set(IMGUI_INCLUDES
	../externals/imgui/imgui.h
	../externals/imgui/backends/imgui_impl_glfw.h
	../externals/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	../externals/imgui/imgui.cpp
	../externals/imgui/backends/imgui_impl_glfw.cpp
	../externals/imgui/backends/imgui_impl_opengl3.cpp
	../externals/imgui/imgui_demo.cpp
	../externals/imgui/imgui_tables.cpp
	../externals/imgui/imgui_widgets.cpp
	../externals/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../externals)
target_include_directories(ImGui PUBLIC ../externals/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)
