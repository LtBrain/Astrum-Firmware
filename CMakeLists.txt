cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set Release optimization flags (does NOT affect Debug)
set(CMAKE_C_FLAGS_RELEASE "-O2 -g -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g -DNDEBUG")

# Set the project name
set(CMAKE_PROJECT_NAME Astrum)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME} C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # C++ sources
    "Core/Src/app.cpp"

    # USB stack sources
    "TINYUSB-SRC/tusb.c"
    "TINYUSB-SRC/common/tusb_fifo.c"
    "TINYUSB-SRC/device/usbd.c"
    "TINYUSB-SRC/device/usbd_control.c"

    "TINYUSB-SRC/class/cdc/cdc_device.c"
    "TINYUSB-SRC/portable/st/stm32_fsdev/dcd_stm32_fsdev.c"
    "TINYUSB-SRC/portable/st/stm32_fsdev/fsdev_common.c"

    "Core/Src/Systems/USB/usb_descriptors.c"
    "Core/Src/Systems/USB/usb.cpp"

    # IMU stack sources
    "Core/Src/Systems/IMU/imu.cpp"
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Inc
    TINYUSB-SRC
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -u _printf_float)

# Make BIN file
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating binary file..."
)

# Fix IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")


# --- Added Release Configuration (no changes to existing Debug setup) ---
set(CMAKE_C_FLAGS_RELEASE "-O2 -g -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g -DNDEBUG")
