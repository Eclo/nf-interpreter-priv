cmake_minimum_required(VERSION 3.0)
ENABLE_LANGUAGE(ASM)

find_package(STM32CMSIS REQUIRED)
find_package(STM32HAL COMPONENTS gpio tim REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis_plus}
    ${STM32CMSIS_INCLUDE_DIRS}
    ${STM32HAL_INCLUDE_DIR}
)

find_package(MICROOSPLUS REQUIRED)
include_directories(${MICROOSPLUS_INCLUDE_DIRS})

# # pass the RTOS option to the compiler flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${RTOS}")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D${RTOS}")


set(PROJECT_SOURCES
    main.c
    initialize_hardware.c
    
    main.cpp    
)

add_executable(
    # executables for project, project sources, STM32 CMSIS sources and STM32 HAL libraries
    ${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES} ${STM32CMSIS_SOURCES} ${STM32HAL_SOURCES} 
    
    # sources for FreeRTOS and STM32 FreeRTOS port
    ${MICROOSPLUS_SOURCES}
)

STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME})

STM32_ADD_HEX_BIN_DUMP_TARGETS(${CMAKE_PROJECT_NAME})

STM32_PRINT_SIZE_OF_TARGETS(${CMAKE_PROJECT_NAME})

# generate map file
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.map")

set(HEX_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(S19_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.s19)
set(BIN_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)
set(DUMP_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.lst)

# generate HEX, BIN and LST files as needed
if(CMAKE_BUILD_TYPE EQUAL "Release")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            # copy target image to other formats
            COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Osrec $<TARGET_FILE:${PROJECT_NAME}> ${S19_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
            
            # copy target file to build folder (this is only usefull for debugging in VS Code because of path in launch.json)
            COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}            
            
            COMMENT "Generate HEX and BIN files fro deployment")
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            # copy target image to other formats
            COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Osrec $<TARGET_FILE:${PROJECT_NAME}> ${S19_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
            
            # copy target file to build folder (this is only usefull for debugging in VS Code because of path in launch.json)
            COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}
            
            # dump target image as source code listing
            COMMAND ${CMAKE_OBJDUMP} -d -EL -S $<TARGET_FILE:${PROJECT_NAME}> > ${DUMP_FILE}
            COMMENT "Generate HEX and BIN files for deployment, LST file for debug")
endif()
