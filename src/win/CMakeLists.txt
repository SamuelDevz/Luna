set(SOURCE_FILES src/Timer.cpp
    src/Input.cpp
    src/Game.cpp
    src/Engine.cpp)

# window library
add_library(window STATIC src/Window.cpp src/Error.cpp)
target_include_directories(window PUBLIC include)
target_compile_definitions(window PUBLIC $<IF:$<CONFIG:DEBUG>,_DEBUG,NDEBUG>)

add_subdirectory(graphics)

# engine library
add_library(engine STATIC ${SOURCE_FILES})
target_link_libraries(engine PRIVATE winmm.lib)
target_link_libraries(engine PUBLIC window graphics)