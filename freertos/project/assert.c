#include <FreeRTOS.h>
#include <task.h>

void vAssertCalled(const char* const pcFileName, unsigned long ulLine) {
  static portBASE_TYPE xPrinted = pdFALSE;
  volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
  /* Parameters are not used. */
  (void) xPrinted;
  (void) ulLine;
  (void) pcFileName;
  taskENTER_CRITICAL();
  {
    /* You can step out of this function to debug the assertion by using
    the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
    value. */
    while( ulSetToNonZeroInDebuggerToContinue == 0 )
    {
    }
  }
  taskEXIT_CRITICAL();
}