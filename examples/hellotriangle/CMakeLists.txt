set(SOURCE_FILES src/Renderer.cpp
    src/Triangle.cpp
    src/main.cpp)

set(RESOURCE_FILES resources/Resources.h 
    resources/Resource.rc)

set(SHADER_FILES resources/Shaders/Vertex.hlsl 
    resources/Shaders/Pixel.hlsl)

set(SHADER_DIRECTORY ${PROJECT_BINARY_DIR}/Shaders)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_executable(triangle WIN32 ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(triangle PUBLIC include resources)
target_link_libraries(triangle PUBLIC Luna::Libraries)
target_link_libraries(triangle PRIVATE d3dcompiler.lib)

# Create directory for HLSL shaders
add_custom_target(directory)
add_custom_command(TARGET directory POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_DIRECTORY}
    COMMAND_EXPAND_LISTS)

# Build HLSL shaders
add_custom_target(shaders)

set_source_files_properties(resources/Shaders/Vertex.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(resources/Shaders/Pixel.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(${SHADER_FILES} PROPERTIES ShaderModel "6_0")

foreach(FILE ${SHADER_FILES})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shadertype ${FILE} ShaderType)
    get_source_file_property(shadermodel ${FILE} ShaderModel)
    add_custom_command(TARGET shaders POST_BUILD
        COMMAND dxc.exe /nologo /Emain /T${shadertype}_${shadermodel}
        $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${SHADER_DIRECTORY}/${FILE_WE}.cso 
        /Fd ${SHADER_DIRECTORY}/${FILE_WE}.pdb ${FILE} /nologo
        BYPRODUCTS 
            ${SHADER_DIRECTORY}/${FILE_WE}.cso
            ${SHADER_DIRECTORY}/${FILE_WE}.pdb
        COMMENT "HLSL ${FILE}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)
endforeach(FILE)

add_dependencies(triangle directory)
add_dependencies(triangle shaders)