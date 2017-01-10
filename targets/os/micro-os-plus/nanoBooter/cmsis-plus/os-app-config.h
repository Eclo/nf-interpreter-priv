#ifndef CMSIS_PLUS_RTOS_OS_APP_CONFIG_H_
#define CMSIS_PLUS_RTOS_OS_APP_CONFIG_H_

// ----------------------------------------------------------------------------

#define OS_INTEGER_SYSTICK_FREQUENCY_HZ                     (1000)

// With 4 bits NVIC, there are 16 levels, 0 = highest, 15 = lowest

// Disable all interrupts from 15 to 4, keep 3-2-1 enabled
#define OS_INTEGER_RTOS_CRITICAL_SECTION_INTERRUPT_PRIORITY (4)

#define OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES               (3000)


#if defined(OS_USE_TRACE_SEGGER_RTT)
#define OS_EXCLUDE_RTOS_IDLE_SLEEP (1)
#endif

#define OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES  (1)
#define OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES        (1)

// ----------------------------------------------------------------------------

#if 0

// Available trace options.
#define OS_TRACE_RTOS_CLOCKS
#define OS_TRACE_RTOS_LISTS_CLOCKS
#define OS_TRACE_RTOS_CONDVAR
#define OS_TRACE_RTOS_EVFLAGS
#define OS_TRACE_RTOS_MEMPOOL
#define OS_TRACE_RTOS_MQUEUE
#define OS_TRACE_RTOS_MUTEX
#define OS_TRACE_RTOS_RTC_TICK
#define OS_TRACE_RTOS_SCHEDULER
#define OS_TRACE_RTOS_SEMAPHORE
#define OS_TRACE_RTOS_SYSCLOCK_TICK
#define OS_TRACE_RTOS_THREAD
#define OS_TRACE_RTOS_THREAD_CONTEXT
#define OS_TRACE_RTOS_THREAD_FLAGS
#define OS_TRACE_RTOS_TIMER

#define OS_TRACE_RTOS_LISTS

#define OS_TRACE_LIBC_MALLOC
#define OS_TRACE_LIBC_ATEXIT

#endif


#define OS_TRACE_RTOS_THREAD
#define OS_TRACE_RTOS_RTC_TICK

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_APP_CONFIG_H_ */
