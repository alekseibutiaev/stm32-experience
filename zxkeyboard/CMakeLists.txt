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
set (PROJECT_NAME                     "zxkeyboard")
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

set(JLINKEXE "/opt/SEGGER/JLink/JLinkExe")
set(ST-UTILS "/usr/bin/st-flash")

option(BOARD_NUCLEO "type of board" ON)
option(MCU_TYPE "MCU type F105R8, F105RB, F105RC, F107RB, F107RC. F105R8 by default" "F105R8")
option(DUMP_ASM "Create full assembly of final executable" OFF)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32${MCU_TYPE}Tx_FLASH.ld)

if (${MCU_TYPE} STREQUAL "F105R8" OR ${MCU_TYPE} STREQUAL "F105RB" OR ${MCU_TYPE} STREQUAL "F105RC")
  set(STARTUP_SCRIPT ${CMAKE_SOURCE_DIR}/startup/startup_stm32f105xc.s)
  set(MCU_MODEL STM32F105xC)
elseif (${MCU_TYPE} STREQUAL "F107RB" OR ${MCU_TYPE} STREQUAL "F107RC")
  set(STARTUP_SCRIPT ${CMAKE_SOURCE_DIR}/startup/startup_stm32f107xc.s)
  set(MCU_MODEL STM32F107xC)
else()
    message(FATAL_ERROR "usupported MCU " ${MCU_TYPE})
endif()

message("MCU_TYPE " ${MCU_TYPE})
message("LINKER_SCRIPT " ${LINKER_SCRIPT})
message("MCU_MODEL " ${MCU_MODEL})
message("STARTUP_SCRIPT " ${STARTUP_SCRIPT})
message("DUMP_ASM " ${DUMP_ASM})

set(PROJECT_INCLUDES
  ${CMAKE_SOURCE_DIR}/Core/Inc
  ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include
  ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
  ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc
  ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
  ${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc
  ${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
  ${CMAKE_SOURCE_DIR}/project
)

file(GLOB_RECURSE STM32CUBEMX_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/Core/Src/*.c
  ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/*.c
  ${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/*.c
  ${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Host_Library/Core/Src/*.c
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

add_executable(${CMAKE_PROJECT_NAME}
  ${STM32CUBEMX_SOURCES}
  ${PROJECT_SOURCES}
  ${STARTUP_SCRIPT}
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:DEBUG>
  $<$<BOOL:${BOARD_NUCLEO}>:NUCLEO>
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
  add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -D $<TARGET_FILE:${CMAKE_PROJECT_NAME}> > ${CMAKE_PROJECT_NAME}.s)
endif()

add_custom_command(OUTPUT jlink_cmd_script
    COMMAND echo "device ${DEVICE}" >> jlink_cmd_script
    COMMAND echo "if swd" >> jlink_cmd_script
    COMMAND echo "Speed 4000" >> jlink_cmd_script
    COMMAND echo "LoadFile ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0x08000000" >> jlink_cmd_script
    COMMAND echo "Exit" >> jlink_cmd_script
    VERBATIM
)

add_custom_target(jlink_upload DEPENDS ${CMAKE_PROJECT_NAME} jlink_cmd_script
    COMMAND cat jlink_cmd_script
    COMMAND ${JLINKEXE} -CommanderScript jlink_cmd_script
)

add_custom_target(stlink_upload DEPENDS ${CMAKE_PROJECT_NAME}
    COMMAND echo "run ${ST-UTILS} write ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0x08000000"
    COMMAND ${ST-UTILS} write ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0x08000000
)

