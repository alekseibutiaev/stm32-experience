/* USER CODE BEGIN Header */
/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */
/* USER CODE END Header */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * These parameters and more are described within the 'configuration' section of the
 * FreeRTOS API documentation available on the FreeRTOS.org web site.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* USER CODE BEGIN Includes */
/* Section where include file can be added */
/* USER CODE END Includes */

/* Ensure definitions are only used by the compiler, and not by the assembler. */

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
  #include <stdint.h>
  extern uint32_t SystemCoreClock;
  void xPortSysTickHandler(void);
#endif
/*
  configUSE_PREEMPTION

  Set to 1 to use the preemptive RTOS scheduler, or 0 to use the cooperative RTOS scheduler.
*/
#define configUSE_PREEMPTION 1

#define configCPU_CLOCK_HZ ( SystemCoreClock )
#define configTICK_RATE_HZ ((TickType_t)1000)
#define configMAX_PRIORITIES                     ( 56 )
#define configMINIMAL_STACK_SIZE                 ((uint16_t)512)
#define configTOTAL_HEAP_SIZE                    ((size_t)2048)
#define configMAX_TASK_NAME_LEN                  ( 16 )
#define configUSE_TRACE_FACILITY                 1
#define configUSE_16_BIT_TICKS                   0
#define configUSE_MUTEXES                        1
#define configQUEUE_REGISTRY_SIZE                8
#define configCHECK_FOR_STACK_OVERFLOW           2
#define configUSE_RECURSIVE_MUTEXES              1
#define configUSE_MALLOC_FAILED_HOOK             1
#define configUSE_DAEMON_TASK_STARTUP_HOOK       1
#define configUSE_COUNTING_SEMAPHORES            1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  0
#define configUSE_TICKLESS_IDLE                  1


//************************************************************************************************//
// Memory allocation related definitions.
//************************************************************************************************//

/*
  configSUPPORT_STATIC_ALLOCATION

  If configSUPPORT_STATIC_ALLOCATION is set to 1 then RTOS objects can be createdusing RAM provided by
  the application writer.

  If configSUPPORT_STATIC_ALLOCATION is set to 0 then RTOS objects can only be created using RAM
  allocated from the FreeRTOS heap.

  If configSUPPORT_STATIC_ALLOCATION is left undefined it will default to 0.

  If configSUPPORT_STATIC_ALLOCATION is set to 1 then the application writer must also provide two
  callback functions: vApplicationGetIdleTaskMemory() to provide the memory for use by the RTOS Idle
  task, and (if configUSE_TIMERS is set to 1) vApplicationGetTimerTaskMemory() to provide memory for
  use by the RTOS Daemon/Timer Service task. Examples are provided below.

  configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
  implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
  used by the Idle task.

  void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
      StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize) {
    // If the buffers to be provided to the Idle task are declared inside this 
    // function then they must be declared static - otherwise they will be allocated on
    // the stack and so not exists after this function exits.
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
    // Pass out a pointer to the StaticTask_t structure in which the Idle task's
    // state will be stored.
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    // Pass out the array that will be used as the Idle task's stack.
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    // Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    // Note that, as the array is necessarily of type StackType_t,
    // configMINIMAL_STACK_SIZE is specified in words, not bytes.
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  }

  configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
  application must provide an implementation of vApplicationGetTimerTaskMemory()
  to provide the memory that is used by the Timer service task.

  void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer,
      StackType_t** ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
    // If the buffers to be provided to the Timer task are declared inside this
    // function then they must be declared static - otherwise they will be allocated on
    // the stack and so not exists after this function exits.
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    // Pass out a pointer to the StaticTask_t structure in which the Timer
    // task's state will be stored.
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    // Pass out the array that will be used as the Timer task's stack.
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    // Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    // Note that, as the array is necessarily of type StackType_t,
    // configTIMER_TASK_STACK_DEPTH is specified in words, not bytes.
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  }
  
  Examples of the callback functions that must be provided by the application to
  supply the RAM used by the Idle and Timer Service tasks if configSUPPORT_STATIC_ALLOCATION
  is set to 1.
*/

#define configSUPPORT_STATIC_ALLOCATION 1

/*
  configSUPPORT_DYNAMIC_ALLOCATION

  If configSUPPORT_DYNAMIC_ALLOCATION is set to 1 then RTOS objects can be created using RAM that is
  automatically allocated from the FreeRTOS heap.

  If configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then RTOS objects can only be created using RAM
  provided by the application writer.

  If configSUPPORT_DYNAMIC_ALLOCATION is left undefined it will default to 1. 
*/

#define configSUPPORT_DYNAMIC_ALLOCATION 1

/*
  configTOTAL_HEAP_SIZE

  The total amount of RAM available in the FreeRTOS heap.

  This value will only be used if configSUPPORT_DYNAMIC_ALLOCATION is set to 1 and the application
  makes use of one of the sample memory allocation schemes provided in the FreeRTOS source code
  download. See the memory configuration section for further details. 
*/

#define configTOTAL_HEAP_SIZE (1024 * 10)

/*
  configAPPLICATION_ALLOCATED_HEAP

  By default the FreeRTOS heap is declared by FreeRTOS and placed in memory by the linker.
  Setting configAPPLICATION_ALLOCATED_HEAP to 1 allows the heap to instead be declared by the
  application writer, which allows the application writer to place the heap wherever they like in
  memory.

  If heap_1.c, heap_2.c or heap_4.c is used, and configAPPLICATION_ALLOCATED_HEAP is set to 1, then
  the application writer must provide a uint8_t array with the exact name and dimension as shown
  below. The array will be used as the FreeRTOS heap. How the array is placed at a specific memory
  location is dependent on the compiler being used - refer to your compiler's documentation.

  uint8_t ucHeap[configTOTAL_HEAP_SIZE];
*/

#define configAPPLICATION_ALLOCATED_HEAP 1

/*
  configSTACK_ALLOCATION_FROM_SEPARATE_HEAP

  If configSTACK_ALLOCATION_FROM_SEPARATE_HEAP is set to 1 then stack for any task created using
  xTaskCreate or xTaskCreateRestricted API is allocated using pvPortMallocStack and freed using
  vPortFreeStack function. The user need to provide thread-safe implementations of pvPortMallocStack
  and vPortFreeStack functions. This enables the user to allocate stacks for tasks from a separate
  region of memory (possibly another heap different from the FreeRTOS heap).

  If configSTACK_ALLOCATION_FROM_SEPARATE_HEAP is left undefined it will default to 0.

  Example implementation of pvPortMallocStack and vPortFreeStack functions is below:
  
  void* pvPortMallocStack(size_t xWantedSize) {
    // Allocate a memory block of size xWantedSize. The function used for
    // allocating memory must be thread safe.
    return MyThreadSafeMalloc( xWantedSize );
  }

  void vPortFreeStack(void* pv) {
    // Free the memory previously allocated using pvPortMallocStack. The
    // function used for freeing the memory must be thread safe.
    MyThreadSafeFree( pv );
  }
*/

#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP 1

//************************************************************************************************//
// Hook function related definitions.                                                             //
//************************************************************************************************//

/*
  configUSE_IDLE_HOOK

  Set to 1 if you wish to use an idle hook, or 0 to omit an idle hook.

  An idle task hook is a function that is called during each cycle of the idle task. If you want
  application functionality to run at the idle priority then there are two options:

    1) Implement the functionality in an idle task hook.

      There must always be at least one task that is ready to run. It is therefore imperative that
      the hook function does not call any API functions that might cause the idle task to block
      (vTaskDelay(), or a queue or semaphore function with a block time, for example). It is ok for
      co-routines to block within the hook function.

    2) Create an idle priority task to implement the functionality.

      This is a more flexible solution but has a higher RAM usage overhead.

  See the (Embedded software application design) <https://www.freertos.org/tutorial/index.html>
  section for more information on using an idle hook.

  To create an idle hook:

    1) Set configUSE_IDLE_HOOK to 1 in FreeRTOSConfig.h.

    2) Define a function that has the following name and prototype:

    void vApplicationIdleHook(void);

  It is common to use the idle hook function to place the microcontroller CPU into a power saving mode. 
*/

#define configUSE_IDLE_HOOK 0

/*
  configUSE_TICK_HOOK

  Set to 1 if you wish to use an tick hook, or 0 to omit an tick hook.

  Tick Hook Function

  The tick interrupt can optionally call an application defined hook (or callback) function - the
  tick hook. The tick hook provides a convenient place to implement timer functionality.

  The tick hook will only get called if configUSE_TICK_HOOK is set to 1 within FreeRTOSConfig.h.
  When this is set the application must provide the hook function with the following prototype:

  void vApplicationTickHook(voi );

  vApplicationTickHook() executes from within an ISR so must be very short, not use much stack, and
  not call any API functions that don't end in "FromISR" or "FROM_ISR".

  See the (demo application file crhook.c) <https://www.freertos.org/a00013.html#crhook> for an
  example of how to use a tick hook.
*/

#define configUSE_TICK_HOOK 0




#define configCHECK_FOR_STACK_OVERFLOW          0
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0
#define configUSE_SB_COMPLETED_CALLBACK         0


//************************************************************************************************//
// Software timer related definitions.                                                            //
//************************************************************************************************//
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               3
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

//************************************************************************************************//
// Interrupt nesting behaviour configuration.                                                     //
//************************************************************************************************//
#if 0
#define configKERNEL_INTERRUPT_PRIORITY         [dependent of processor]
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    [dependent on processor and application]
#define configMAX_API_CALL_INTERRUPT_PRIORITY   [dependent on processor and application]
#endif

//************************************************************************************************//
// Define to trap errors during development.                                                      //
//************************************************************************************************//
#define configASSERT( ( x ) ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

//************************************************************************************************//
// FreeRTOS MPU specific definitions.                                                             //
//************************************************************************************************//
#if 0
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0
#define configTOTAL_MPU_REGIONS                                8 /* Default value. */
#define configTEX_S_C_B_FLASH                                  0x07UL /* Default value. */
#define configTEX_S_C_B_SRAM                                   0x07UL /* Default value. */
#define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY            1
#define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS             1
#define configENABLE_ERRATA_837070_WORKAROUND                  1
#define configUSE_MPU_WRAPPERS_V1                              0
#define configPROTECTED_KERNEL_OBJECT_POOL_SIZE                10
#define configSYSTEM_CALL_STACK_SIZE                           128
#endif

//************************************************************************************************//
// ARMv8-M secure side port related definitions.                                                  //
//************************************************************************************************//
#if 0
#define secureconfigMAX_SECURE_CONTEXTS         5
#endif

//************************************************************************************************//
// Optional functions - most linkers will remove unused functions anyway.                         //
//************************************************************************************************//
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_uxTaskGetStackHighWaterMark2    0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

#endif /* FREERTOS_CONFIG_H */

#if 0
configADJUSTED_HEAP_SIZE
configAPPLICATION_ALLOCATED_HEAP
configASSERT
configASSERT_DEFINED
configCHECK_FOR_STACK_OVERFLOW
configCPU_CLOCK_HZ
configDEINIT_TLS_BLOCK
configENABLE_BACKWARD_COMPATIBILITY
configENABLE_FPU
configENABLE_MPU
configENABLE_MVE
configENABLE_TRUSTZONE
configEXPECTED_IDLE_TIME_BEFORE_SLEEP
configGENERATE_RUN_TIME_STATS
configHEAP_CLEAR_MEMORY_ON_FREE
configIDLE_SHOULD_YIELD
configIDLE_TASK_NAME
configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
configINITIAL_TICK_COUNT
configINIT_TLS_BLOCK
configKERNEL_INTERRUPT_PRIORITY
configLIST_VOLATILE
configMAX_CO_ROUTINE_PRIORITIES
configMAX_PRIORITIES
configMAX_SYSCALL_INTERRUPT_PRIORITY
configMAX_TASK_NAME_LEN
configMESSAGE_BUFFER_LENGTH_TYPE
configMINIMAL_SECURE_STACK_SIZE
configMINIMAL_STACK_SIZE
configNUM_THREAD_LOCAL_STORAGE_POINTERS
configPRECONDITION
configPRECONDITION_DEFINED
configPRIO_BITS
configQUEUE_REGISTRY_SIZE
configRECORD_STACK_HIGH_ADDRESS
configRUN_ADDITIONAL_TESTS
configRUN_FREERTOS_SECURE_ONLY
configRUN_TIME_COUNTER_TYPE
configSET_TLS_BLOCK
configSTACK_ALLOCATION_FROM_SEPARATE_HEAP
configSTACK_DEPTH_TYPE
configSUPPORT_CROSS_MODULE_OPTIMISATION
configSUPPORT_DYNAMIC_ALLOCATION
configSUPPORT_STATIC_ALLOCATION
configSYSTICK_CLOCK_HZ
configTASK_NOTIFICATION_ARRAY_ENTRIES
configTICK_RATE_HZ
configTIMER_QUEUE_LENGTH
configTIMER_SERVICE_TASK_NAME
configTIMER_TASK_PRIORITY
configTIMER_TASK_STACK_DEPTH
configTLS_BLOCK_TYPE
configTOTAL_HEAP_SIZE
configUSE_ALTERNATIVE_API
configUSE_APPLICATION_TASK_TAG
configUSE_CO_ROUTINES
configUSE_COUNTING_SEMAPHORES
configUSE_C_RUNTIME_TLS_SUPPORT
configUSE_DAEMON_TASK_STARTUP_HOOK
configUSE_IDLE_HOOK
configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES
configUSE_MALLOC_FAILED_HOOK
configUSE_MINI_LIST_ITEM
configUSE_MUTEXES
configUSE_NEWLIB_REENTRANT
configUSE_PORT_OPTIMISED_TASK_SELECTION
configUSE_POSIX_ERRNO
configUSE_PREEMPTION
configUSE_QUEUE_SETS
configUSE_RECURSIVE_MUTEXES
configUSE_SB_COMPLETED_CALLBACK
configUSE_STATS_FORMATTING_FUNCTIONS
configUSE_TASK_FPU_SUPPORT
configUSE_TASK_NOTIFICATIONS
configUSE_TICK_HOOK
configUSE_TICKLESS_IDLE
configUSE_TIMERS
configUSE_TIME_SLICING
configUSE_TRACE_FACILITY
#endif