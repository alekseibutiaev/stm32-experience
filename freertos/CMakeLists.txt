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
set (PROJECT_NAME                     "freertos")
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
set(MCPU                     ${MCPU_CORTEX_M3})
set(MFLOAT_ABI               ${MFLOAT_ABI_SOFTWARE})
set(RUNTIME_LIBRARY          ${RUNTIME_LIBRARY_REDUCED_C})
set(RUNTIME_LIBRARY_SYSCALLS ${RUNTIME_LIBRARY_SYSCALLS_MINIMAL})
set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(TAG_MASK "frtos_*")

# Skip link step during toolchain validation.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_package(Git)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

option(DUMP_ASM "Create full assembly of final executable" OFF)

set(FREERTOS_HEAP "4" CACHE STRING "use heap type. by default 4")
set(FreeRTOS_VERSION "V10.0.1" CACHE STRING "select FreeRTOS version. by default FreeRTOS V10.0.1")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32F103ZETx_FLASH.ld)
set(STARTUP ${CMAKE_SOURCE_DIR}/Core/Startup/startup_stm32f103xe.s)
set(MCU_MODEL STM32F103xE)

add_library(freertos_config INTERFACE) 
target_include_directories(freertos_config SYSTEM INTERFACE ${FREERTOS_CONFIG_FILE_DIRECTORY} )
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)

message("FreeRTOS_VERSION: " ${FreeRTOS_VERSION})
message("FREERTOS_HEAP: " ${FREERTOS_HEAP})
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
  ${CMAKE_SOURCE_DIR}/freertos
  ${CMAKE_SOURCE_DIR}/freertos/${FREERTOS_VERSION}/include
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

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set(CMAKE_ASM_FLAGS "${MCPU} -x assembler-with-cpp $")

project(${PROJECT_NAME})

enable_language(ASM C CXX)

add_custom_target(version
  ${CMAKE_COMMAND}
    -D SRC=${CMAKE_SOURCE_DIR}/version.h.in
    -D DST=${CMAKE_BINARY_DIR}/version.h
    -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
    -D TAG=${TAG_MASK}
    -D NAME=${PROJECT_NAME}
    -P ${CMAKE_MODULE_PATH}GenerateVersionHeader.cmake
)

add_executable(${CMAKE_PROJECT_NAME}
  ${STM32CUBEMX_SOURCES}
  ${PROJECT_SOURCES}
  ${STARTUP}
)

add_dependencies(${CMAKE_PROJECT_NAME} version)

target_link_libraries(${CMAKE_PROJECT_NAME}
  freertos_kernel
  freertos_kernel_port
)

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
  -Wl,--gc-sections
  -static
  ${MFLOAT_ABI}
  -mthumb
  -Wl,--start-group
  -lc
  -lm
  -Wl,--end-group
  -Wl,--print-memory-usage
)

add_subdirectory(freertos/${FreeRTOS_VERSION})

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)

if (${DUMP_ASM})
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -D $<TARGET_FILE:${CMAKE_PROJECT_NAME}> > ${CMAKE_PROJECT_NAME}.s)
endif()

# keep example upload for jlink
set(JLINKEXE "/opt/SEGGER/JLink/JLinkExe")

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

# keep example upload for st-utils
set(ST-UTILS "/usr/bin/st-flash")

add_custom_target(stlink_upload DEPENDS ${CMAKE_PROJECT_NAME}
  COMMAND echo "run ${ST-UTILS} write ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0x08000000"
  COMMAND ${ST-UTILS} write ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0x08000000
)

add_custom_command(
  OUTPUT upload_cmd
  DEPENDS ${CMAKE_PROJECT_NAME}
  COMMAND touch upload_cmd
  COMMAND openocd -f ${CMAKE_SOURCE_DIR}/openocd.cfg/software_system_reset.cfg
    -c \"program ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0x08000000 verify reset exit\"
)

add_custom_target(upload DEPENDS upload_cmd)
