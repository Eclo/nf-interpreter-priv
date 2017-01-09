//---------------------------------------------------------------------------
// Copyright (c) 2016 The nano Framework Interpreter project contributors
//---------------------------------------------------------------------------
#include "ch.h"
#include "hal.h"
#include "usbcfg.h"
#include "cmsis_os.h"
// #include <WireProtocol.h>

//extern UINT32 SUPPORT_ComputeCRC(const void *rgBlock, int nLength, UINT32 crc);

static uint8_t buffer[240];

static void BlinkerThread(void const *arg) {

  (void)arg;

  while (true) {
      palSetPad(GPIOD, GPIOD_LED3);
      osDelay(250);

      palClearPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      osDelay(250);

      palClearPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      osDelay(250);

      palClearPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      osDelay(250);
      
      palClearPad(GPIOD, GPIOD_LED4);        
  }
}
osThreadDef(BlinkerThread, osPriorityAboveNormal, 256);

static void ReceiverThread(void const *arg) {
  uint8_t bytesRead = 0;
 
  (void)arg;
  char c;
  char *p = buffer;

  // TODO change algorithm bellow to read expected number of chars for message, possibly with a timeout
  while (true) {

    // reads one char at a time
      if(chSequentialStreamRead((BaseSequentialStream *)&SDU1, (uint8_t *)&c, 1) != 0)
      {
      
        *p++ = c;
      }

      if(bytesRead  > 0)
      {

      }
      //osDelay(250);


  }
}
osThreadDef(ReceiverThread, osPriorityAboveNormal, 256);

//  Application entry point.
int main(void) {

  //   HAL initialization, this also initializes the configured device drivers
  //      and performs the board-specific initializations.
  halInit();

  //  The kernel is initialized but not started yet, this means that
  //    main() is executing with absolute priority but interrupts are
  //    already enabled.
  osKernelInitialize();

  //  Initializes a serial-over-USB CDC driver.
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  // Activates the USB driver and then the USB bus pull-up on D+.
  // Note, a delay is inserted in order to not have to disconnect the cable
  // after a reset.
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  // creates the blinker thread, but doesn't start it
  osThreadCreate(osThread(BlinkerThread), NULL);

  // creates the blinker thread, but doesn't start it
  osThreadCreate(osThread(ReceiverThread), NULL);

  // Kernel started, the main() thread has priority osPriorityNormal
  //    by default.
  osKernelStart();

  // In the ChibiOS/RT CMSIS RTOS implementation the main() is an
  //    usable thread, here we just sleep in a loop printing a message.
  while (true) {
    osDelay(500);
  }
}
