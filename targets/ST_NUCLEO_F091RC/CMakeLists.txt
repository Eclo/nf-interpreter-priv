cmake_minimum_required(VERSION 3.0)
ENABLE_LANGUAGE(ASM)

include_directories(
     ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(CHIBIOS REQUIRED)
include_directories(${CHIBIOS_INCLUDE_DIRS})
# include dirs for Wire Protocol
include_directories(${WIRE_PROTOCOL_INCLUDE_DIRS})
# include dirs for ChibiOS nanoFramework interface
include_directories(${CHIBIOS_NANO_INTERFACE_INCLUDE_DIRS})

#######################
# nanoBooter executable 

set(NANOBOOTER_PROJECT_NAME "nanoBooter")

set(NANOBOOTER_PROJECT_SOURCES
    main_nanoBooter.c  
)

add_executable(
    # executables for project, project sources 
    ${NANOBOOTER_PROJECT_NAME} ${NANOBOOTER_PROJECT_SOURCES}
    
    # sources for ChibiOS
    ${CHIBIOS_SOURCES}

    # sources for ChibiOS nanoFramework interface
    ${CHIBIOS_NANO_INTERFACE_SOURCES}

    # sources for Wire Protocol
    ${WIRE_PROTOCOL_SOURCES}
)

#######################
# nanoCLR executable 

set(NANOCLR_PROJECT_NAME "nanoCLR")

set(NANOCLR_PROJECT_SOURCES
    main_nanoCLR.c  
)

add_executable(
    # executables for project, project sources 
    ${NANOCLR_PROJECT_NAME} ${NANOCLR_PROJECT_SOURCES}
    
    # sources for ChibiOS
    ${CHIBIOS_SOURCES}

    # sources for ChibiOS nanoFramework interface
    ${CHIBIOS_NANO_INTERFACE_SOURCES}

    # sources for Wire Protocol
    ${WIRE_PROTOCOL_SOURCES}
)

CHIBIOS_SET_LINKER_OPTIONS(${NANOBOOTER_PROJECT_NAME})
CHIBIOS_SET_LINKER_OPTIONS(${NANOCLR_PROJECT_NAME})

CHIBIOS_ADD_HEX_BIN_DUMP_TARGETS(${NANOBOOTER_PROJECT_NAME})
CHIBIOS_ADD_HEX_BIN_DUMP_TARGETS(${NANOCLR_PROJECT_NAME})

CHIBIOS_PRINT_SIZE_OF_TARGETS(${NANOBOOTER_PROJECT_NAME})
CHIBIOS_PRINT_SIZE_OF_TARGETS(${NANOCLR_PROJECT_NAME})

# set linker flags for DEBUG
set_property(TARGET ${NANOBOOTER_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_DEBUG "")
set_property(TARGET ${NANOCLR_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_DEBUG "")

# set linker flags for RELEASE
set_property(TARGET ${NANOBOOTER_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_RELEASE " -flto ")
set_property(TARGET ${NANOCLR_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_RELEASE " -flto ")

# add linker flags set with traditional CMAKE_EXE_LINKER_FLAGS
set_property(TARGET ${NANOBOOTER_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
set_property(TARGET ${NANOCLR_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

# add linker flags to generate map file
set_property(TARGET ${NANOBOOTER_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${NANOBOOTER_PROJECT_NAME}.map")
set_property(TARGET ${NANOCLR_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${NANOCLR_PROJECT_NAME}.map")

# add other linker flags 
set_property(TARGET ${NANOBOOTER_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS ",--library-path=${PROJECT_BINARY_DIR}/ChibiOS_Source/common/ports/ARMCMx/compilers/GCC,--defsym=__main_stack_size__=0x400,--defsym=__process_stack_size__=0x400")
set_property(TARGET ${NANOCLR_PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS ",--library-path=${PROJECT_BINARY_DIR}/ChibiOS_Source/common/ports/ARMCMx/compilers/GCC,--defsym=__main_stack_size__=0x400,--defsym=__process_stack_size__=0x400")


set(NANOBOOTER_HEX_FILE ${PROJECT_SOURCE_DIR}/build/${NANOBOOTER_PROJECT_NAME}.hex)
set(NANOCLR_HEX_FILE ${PROJECT_SOURCE_DIR}/build/${NANOCLR_PROJECT_NAME}.hex)
set(NANOBOOTER_S19_FILE ${PROJECT_SOURCE_DIR}/build/${NANOBOOTER_PROJECT_NAME}.s19)
set(NANOCLR_S19_FILE ${PROJECT_SOURCE_DIR}/build/${NANOCLR_PROJECT_NAME}.s19)
set(NANOBOOTER_BIN_FILE ${PROJECT_SOURCE_DIR}/build/${NANOBOOTER_PROJECT_NAME}.bin)
set(NANOCLR_BIN_FILE ${PROJECT_SOURCE_DIR}/build/${NANOCLR_PROJECT_NAME}.bin)
set(NANOBOOTER_DUMP_FILE ${PROJECT_SOURCE_DIR}/build/${NANOBOOTER_PROJECT_NAME}.lst)
set(NANOCLR_DUMP_FILE ${PROJECT_SOURCE_DIR}/build/${NANOCLR_PROJECT_NAME}.lst)

# generate HEX, BIN and LST files as needed
if(CMAKE_BUILD_TYPE EQUAL "Release")

    add_custom_command(TARGET ${NANOBOOTER_PROJECT_NAME} POST_BUILD
            # copy target image to other formats
            COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${NANOBOOTER_PROJECT_NAME}> ${NANOBOOTER_HEX_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Osrec $<TARGET_FILE:${NANOBOOTER_PROJECT_NAME}> ${NANOBOOTER_S19_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${NANOBOOTER_PROJECT_NAME}> ${NANOBOOTER_BIN_FILE}
            
            # copy target file to build folder (this is only usefull for debugging in VS Code because of path in launch.json)
            COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${NANOBOOTER_PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/build/${NANOBOOTER_PROJECT_NAME}            
            
            COMMENT "Generate nanoBooter HEX and BIN files for deployment")

    add_custom_command(TARGET ${NANOCLR_PROJECT_NAME} POST_BUILD
            # copy target image to other formats
            COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${NANOCLR_PROJECT_NAME}> ${NANOCLR_HEX_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Osrec $<TARGET_FILE:${NANOCLR_PROJECT_NAME}> ${NANOCLR_S19_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${NANOCLR_PROJECT_NAME}> ${NANOCLR_BIN_FILE}
            
            # copy target file to build folder (this is only usefull for debugging in VS Code because of path in launch.json)
            COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${NANOCLR_PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/build/${NANOCLR_PROJECT_NAME}            
            
            COMMENT "Generate nanoCLR HEX and BIN files for deployment")

else()

    add_custom_command(TARGET ${NANOBOOTER_PROJECT_NAME} POST_BUILD
            # copy target image to other formats
            COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${NANOBOOTER_PROJECT_NAME}> ${NANOBOOTER_HEX_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Osrec $<TARGET_FILE:${NANOBOOTER_PROJECT_NAME}> ${NANOBOOTER_S19_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${NANOBOOTER_PROJECT_NAME}> ${NANOBOOTER_BIN_FILE}
            
            # copy target file to build folder (this is only usefull for debugging in VS Code because of path in launch.json)
            COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${NANOBOOTER_PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/build/${NANOBOOTER_PROJECT_NAME}
            
            # dump target image as source code listing
            COMMAND ${CMAKE_OBJDUMP} -d -EL -S $<TARGET_FILE:${NANOBOOTER_PROJECT_NAME}> > ${NANOBOOTER_DUMP_FILE}

            COMMENT "Generate nanoBooter HEX and BIN files for deployment, LST file for debug")

    add_custom_command(TARGET ${NANOCLR_PROJECT_NAME} POST_BUILD
            # copy target image to other formats
            COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${NANOCLR_PROJECT_NAME}> ${NANOCLR_HEX_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Osrec $<TARGET_FILE:${NANOCLR_PROJECT_NAME}> ${NANOCLR_S19_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${NANOCLR_PROJECT_NAME}> ${NANOCLR_BIN_FILE}
            
            # copy target file to build folder (this is only usefull for debugging in VS Code because of path in launch.json)
            COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${NANOCLR_PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/build/${NANOCLR_PROJECT_NAME}
            
            # dump target image as source code listing
            COMMAND ${CMAKE_OBJDUMP} -d -EL -S $<TARGET_FILE:${NANOCLR_PROJECT_NAME}> > ${NANOCLR_DUMP_FILE}

            COMMENT "Generate nanoCLR HEX and BIN files for deployment, LST file for debug")

endif()
