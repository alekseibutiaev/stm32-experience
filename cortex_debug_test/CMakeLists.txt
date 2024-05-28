#############################################################################################################################
# file:  CMakeLists.txt
# brief: Template "CMakeLists.txt" for building of executables and static libraries.
#
# usage: Edit "VARIABLES"-section to suit project requirements.
#        For debug build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#          make -C Debug VERBOSE=1
#        For release build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
#          make -C Release VERBOSE=1
#############################################################################################################################
cmake_minimum_required(VERSION 3.20)

###################### CONSTANTS ######################################
set (PROJECT_NAME                     "cortex-debug-test")
set (PROJECT_TYPE_EXECUTABLE          "exe")
set (PROJECT_TYPE_STATIC_LIBRARY      "static-lib")
set (MCPU_CORTEX_M0				            "-mcpu=cortex-m0")
set (MCPU_CORTEX_M0PLUS				        "-mcpu=cortex-m0plus")
set (MCPU_CORTEX_M3				            "-mcpu=cortex-m3")
set (MCPU_CORTEX_M4				            "-mcpu=cortex-m4")
set (MCPU_CORTEX_M7				            "-mcpu=cortex-m7")
set (MCPU_CORTEX_M33				          "-mcpu=cortex-m33")
set (MFPU_FPV4_SP_D16                 "-mfpu=fpv4-sp-d16")
set (MFPU_FPV5_D16                    "-mfpu=fpv5-d16")
set (RUNTIME_LIBRARY_REDUCED_C        "--specs=nano.specs")
set (RUNTIME_LIBRARY_STD_C            "")
set (RUNTIME_LIBRARY_SYSCALLS_MINIMAL "--specs=nosys.specs")
set (RUNTIME_LIBRARY_SYSCALLS_NONE    "")
set (MFLOAT_ABI_SOFTWARE              "-mfloat-abi=soft")
set (MFLOAT_ABI_HARDWARE              "-mfloat-abi=hard")
set (MFLOAT_ABI_MIX                   "-mfloat-abi=softfp")
#######################################################################

###################### VARIABLES ######################################
set(MCPU                     "-mcpu=Cortex-M3")
set(MFLOAT_ABI               ${MFLOAT_ABI_SOFTWARE})
set(RUNTIME_LIBRARY          "--specs=nano.specs")
set(RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")
set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

find_package(Git)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

option(DUMP_ASM "Create full assembly of final executable" OFF)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32F103ZETx_FLASH.ld)

set(STARTUP ${CMAKE_SOURCE_DIR}/startup/startup_stm32f103xe.s)
set(MCU_MODEL STM32F103xE)

message("MCU_MODEL: " ${MCU_MODEL})
message("STARTUP: " ${STARTUP})
message("LINKER_SCRIPT: " ${LINKER_SCRIPT})
message("DUMP_ASM: " ${DUMP_ASM})

set(PROJECT_INCLUDES
  ${CMAKE_SOURCE_DIR}/Core/Inc
  ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include
  ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
  ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc
  ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
  ${CMAKE_SOURCE_DIR}/project
  ${CMAKE_BINARY_DIR}
)

file(GLOB_RECURSE STM32CUBEMX_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/Core/Src/*.c
  ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/*.c
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/project/*.c
  ${CMAKE_SOURCE_DIR}/project/*.cpp
)

############ MODIFY ACCORDING TO REQUIREMENTS) ########################

#######################################################################


################## PROJECT SETUP ######################################

#include_directories(${PROJECT_INCLUDES})

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")


project(${PROJECT_NAME})
enable_language(ASM C CXX)

add_custom_target(version
  ${CMAKE_COMMAND}
    -D SRC=${CMAKE_SOURCE_DIR}/version.h.in
    -D DST=${CMAKE_BINARY_DIR}/version.h
    -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
    -D NAME=${PROJECT_NAME}
    -P ${CMAKE_MODULE_PATH}GenerateVersionHeader.cmake
)

add_executable(${CMAKE_PROJECT_NAME}
  ${STM32CUBEMX_SOURCES}
  ${PROJECT_SOURCES}
  ${STARTUP}
)

add_dependencies(${CMAKE_PROJECT_NAME} version)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:DEBUG>
  ${MCU_MODEL}
  USE_HAL_DRIVER
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${PROJECT_INCLUDES}
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
  -Wall
  -fstack-usage
  -fdata-sections
  -ffunction-sections
  $<$<COMPILE_LANGUAGE:CXX>:
    -Wconversion # STM libraries!
    -Wno-volatile
    -Wold-style-cast
    -Wuseless-cast
    -Wsuggest-override>
  $<$<CONFIG:Debug>:-O0 -g3>
  $<$<CONFIG:Release>:-Os -g0>
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
  -T${LINKER_SCRIPT}
  ${RUNTIME_LIBRARY_SYSCALLS}
  -Wl,-Map=${CMAKE_PROJECT_NAME}.map
  -Wl,--gc-sections -static
  -Wl,--start-group
  -lc
  -lm
  -Wl,--end-group
  -Wl,--print-memory-usage
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)

if (${DUMP_ASM})
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -D $<TARGET_FILE:${CMAKE_PROJECT_NAME}> > ${CMAKE_PROJECT_NAME}.s)
endif()

add_custom_command(
  OUTPUT ocd_upload
  DEPENDS ${CMAKE_PROJECT_NAME}
  COMMAND touch ocd_upload
  COMMAND echo "run: openocd -f ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.cfg -c \"init; reset halt; flash write_image erase ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0x08000000; reset; shutdown\""
  COMMAND openocd -f ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.cfg
    -c \"init\; reset halt\; flash write_image erase ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0x08000000\; reset run\"
)

add_custom_target(upload DEPENDS ocd_upload)

