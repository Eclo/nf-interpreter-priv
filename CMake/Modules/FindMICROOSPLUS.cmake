# set include directories for µOS++ III
list(APPEND MICROOSPLUS_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source)
list(APPEND MICROOSPLUS_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/include)
list(APPEND MICROOSPLUS_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/libc/stdlib)

list(APPEND MICROOSPLUS_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/micro-os-plus-iii-cortexm_Source)
list(APPEND MICROOSPLUS_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/micro-os-plus-iii-cortexm_Source/include)
list(APPEND MICROOSPLUS_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/micro-os-plus-iii-cortexm_Source/include/cmsis-plus/rtos/port)


# source files for MICROOSPLUS
set(MICROOSPLUS_SRCS
    # diag
    trace.cpp
    trace-itm.cpp
    trace-segger-rtt.cpp
    trace-semihosting.cpp

    # libc
    c-newlib-reent.cpp
    assert.c
    atexit.cpp
    exit.c
    malloc.cpp
    _sbrk.c

    # libcpp
    chrono.cpp
    condition-variable.cpp
    cxx.cpp
    memory-resource.cpp
    mutex.cpp
    new.cpp
    system-error.cpp
    thread.cpp

    # memory
    block-pool.cpp
    first-fit-top.cpp
    lifo.cpp

    # posix-io
    c-syscalls-posix.cpp
    device-char.cpp
    device-char-registry.cpp
    directory.cpp
    file.cpp
    file-descriptors-manager.cpp
    file-system.cpp
    io.cpp
    mount-manager.cpp
    net-stack.cpp
    pool.cpp
    socket.cpp

    # rtos
    os-flags.cpp
    os-lists.cpp
    os-c-wrapper.cpp
    os-clocks.cpp
    os-condvar.cpp
    os-core.cpp
    os-evflags.cpp
    os-idle.cpp
    os-main.cpp
    os-memory.cpp
    os-mempool.cpp
    os-mqueue.cpp
    os-mutex.cpp
    os-semaphore.cpp
    os-thread.cpp
    os-timer.cpp

    # startup
    exception-handlers.c
    initialise-free-store.cpp
    initialize-hardware.c
    reset-hardware.c
    startup.cpp

    # utils
    lists.cpp
)

foreach(SRC_FILE ${MICROOSPLUS_SRCS})
    set(MICROOSPLUS_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(MICROOSPLUS_SRC_FILE ${SRC_FILE}
        PATHS 
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/diag
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/libc
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/libc/newlib
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/libc/stdlib
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/libcpp
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/memory
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/posix-io
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/rtos
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/rtos/internal
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/startup
            ${PROJECT_BINARY_DIR}/micro-os-plus-iii_Source/src/utils

        CMAKE_FIND_ROOT_PATH_BOTH
    )
    # message("${SRC_FILE} >> ${MICROOSPLUS_SRC_FILE}") # debug helper
    list(APPEND MICROOSPLUS_SOURCES ${MICROOSPLUS_SRC_FILE})
endforeach()

# include source for CortexM port
list(APPEND MICROOSPLUS_SOURCES ${PROJECT_BINARY_DIR}/micro-os-plus-iii-cortexm_Source/src/rtos/port/os-core.cpp)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(MICROOSPLUS DEFAULT_MSG MICROOSPLUS_INCLUDE_DIRS MICROOSPLUS_SOURCES)
