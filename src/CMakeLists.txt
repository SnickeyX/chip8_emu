set(target chip8_emulator)
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${target} ${headers} ${code})
target_include_directories(${target}
    INTERFACE
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(${target}
    chip8_lib
    ${CURSES_LIBRARIES}
)
set_target_properties(chip8_emulator
PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/files"
)