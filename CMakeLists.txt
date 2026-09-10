cmake_minimum_required(VERSION 3.20)

# must be set *before* project - configure for arm-none-eabi for crosscompiling
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/toolchain-arm-none-eabi.cmake
    CACHE FILEPATH "toolchain file")

project(blinky LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CPU flags (shared by the compiler and the linker)
set(MCU_FLAGS
    -mcpu=cortex-m0plus
    -mthumb
    -mfloat-abi=soft
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/py32f030.ld)

file(GLOB LL_DRIVER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/LL/Source/*.c"
)
add_executable(${PROJECT_NAME}
    CMSIS/startup_py32f030.S
    CMSIS/PY32F0xx/Source/system_py32f0xx.c
    ${LL_DRIVER_SOURCES}
    src/platform_usart.cpp
)

# important for Puya's CMSIS libraries so that they know what exact chip to use
target_compile_definitions(blinky PRIVATE
    PY32F030x8
    USE_FULL_LL_DRIVER
)

# CMSIS include folders
target_include_directories(blinky PRIVATE
    ${CMAKE_SOURCE_DIR}/CMSIS/PY32F0xx/Include
    ${CMAKE_SOURCE_DIR}/CMSIS/Core/Include
    ${CMAKE_SOURCE_DIR}/LL/Include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -ffreestanding
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
    # C++-only flags: no runtime type info / exception unwinder on bare metal
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti -fno-threadsafe-statics>
    $<$<CONFIG:Debug>:-Og -g3>
    $<$<CONFIG:Release>:-O2>
)

target_link_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -T${LINKER_SCRIPT}
    -nostartfiles                # startup_py32f030.S provides Reset_Handler/vectors instead
    -specs=nano.specs            # newlib-nano: smaller libc for embedded targets
    -specs=nosys.specs           # stub out syscalls (_write, _sbrk, ...) - no OS
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
)

# Re-link automatically if the linker script changes.
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT}
    SUFFIX ".elf"
)

# Produce .hex/.bin alongside the .elf and print a size summary, same as a
# typical Keil/CubeIDE post-build step.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
    BYPRODUCTS
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMENT "Generating ${PROJECT_NAME}.hex / .bin"
)



