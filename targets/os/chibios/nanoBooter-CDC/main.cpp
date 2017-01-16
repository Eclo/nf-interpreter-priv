//---------------------------------------------------------------------------
// Copyright (c) 2016 The nano Framework Interpreter project contributors
//---------------------------------------------------------------------------
#include "ch.hpp"
#include "hal.h"
#include "usbcfg.h"
// #include "cmsis_os.h"
#include <WireProtocol.h>

using namespace chibios_rt;

extern UINT32 SUPPORT_ComputeCRC(const void *rgBlock, int nLength, UINT32 crc);

static uint8_t buffer[240];

class BlinkerThread : public BaseStaticThread<128> {
private:
protected:
  virtual void main(void) {

    setName("blinker");

    while (true) {
      palSetPad(GPIOD, GPIOD_LED3);
      sleep(MS2ST(500));

      palClearPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      sleep(MS2ST(500));

      palClearPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      sleep(MS2ST(500));

      palClearPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      sleep(MS2ST(500));
      
      palClearPad(GPIOD, GPIOD_LED4);        
    }
  }

public:
  BlinkerThread() : BaseStaticThread<128>() {}
};


class Receiver : public BaseStaticThread<1024> {

protected:
  virtual void main(void) {

    setName("Receiver");

    msg->PrepareReception();

    while (true) {
      process();
    }
  }

public:
  Receiver(WP_Message *msg) {
    this->msg = msg;
    this->msg->Initialize(NULL);

    m_lastOutboundMessage = 0;
  }

private:
  bool process() {
    //
    // UNDONE: FIXME: Copy of WireProtocol.cpp code
    //
    while (true) {

      switch (msg->m_rxState) {
      case WP_Message::ReceiveState::Idle:
        // return true;
        msg->m_rxState = WP_Message::ReceiveState::Initialize;
        break;
      case WP_Message::ReceiveState::Initialize:
        msg->m_rxState = WP_Message::ReceiveState::WaitingForHeader;
        msg->m_pos = (UINT8 *)&msg->m_header;
        msg->m_size = sizeof(msg->m_header);
        break;

      case WP_Message::ReceiveState::WaitingForHeader:
        if (!readData()) {
          // return true;
          msg->m_rxState = WP_Message::ReceiveState::Idle;
          break;
        }
        if (syncToMessageStart()) {
          msg->m_rxState = WP_Message::ReceiveState::ReadingHeader;
        }
        break;

      case WP_Message::ReceiveState::ReadingHeader:
        if (readData()) {
          msg->m_rxState = WP_Message::ReceiveState::CompleteHeader;
        }
        break;

      case WP_Message::ReceiveState::CompleteHeader:
        // UNDONE: VerifyHeader
        // if(processHeader())
        {
          if (msg->m_header.m_size) {
            // UNDONE: Receive payload
          } else {
            msg->m_rxState = WP_Message::ReceiveState::CompletePayload;
          }
        }
        // UNDONE: fBadPacket
        break;

      case WP_Message::ReceiveState::CompletePayload:
        // UNDONE: VerifyPayload

        processPayload();
        // else ReplyBadPacket

        msg->m_rxState = WP_Message::ReceiveState::Initialize;
        break;

      default:
        // Unknown state
        // return false;
        msg->m_rxState = WP_Message::ReceiveState::Idle;
        break;
      }
    }
  }

  WP_Message *msg;
  uint8_t m_buffer[256];
  int m_lastOutboundMessage;

  bool syncToMessageStart() {
    int len;
    while (true) {
      len = sizeof(msg->m_header) - msg->m_size;
      if (len <= 0) {
        break;
      }

      size_t lenCmp = min(len, sizeof(msg->m_header.m_signature));

      if (memcmp(&msg->m_header, MARKER_DEBUGGER_V1, lenCmp) == 0) {
        break;
      }
      if (memcmp(&msg->m_header, MARKER_PACKET_V1, lenCmp) == 0) {
        break;
      }

      UINT8 *buff = (UINT8 *)&msg->m_header;

      memmove(buff, &buff[1], len - 1);

      msg->m_pos--;
      msg->m_size++;
    }
    return (len >= sizeof(msg->m_header.m_signature));
  }

  bool readData() {

    volatile  UINT8 bytesRead __attribute__((used)) = 0;

    char c;
    uint8_t *p = buffer;
    UINT16 timerCounter = 0;

    if (SDU1.config->usbp->state == USB_ACTIVE)
    {

      while(msg->m_size > 0 || timerCounter < 3000)
      {
          // 

//           *p++ = (uint8_t)chSequentialStreamGet((BaseSequentialStream *)&SDU1);
// msg->m_size--;


          bytesRead = chSequentialStreamRead((BaseSequentialStream *)&SDU1, (uint8_t *)msg->m_pos, msg->m_size);
          if(bytesRead != 0)
           {
              msg->m_size -= bytesRead;

              if(msg->m_size == 0)
              {
                return TRUE;
              }
          }

          // there are more chars to read so pause a tiny bit 
          sleep(MS2ST(100));
          // add time
          timerCounter += 100;
      }
    }
    else
    {
        // USB is not active, sleep here during a second
        sleep(MS2ST(1000));
    }

    return msg->m_size == 0;
  }

  void processPayload() {
    WP_Message reply;

    reply.Initialize(NULL);
    reply.m_header.m_flags |= WP_Flags::c_ACK;
    reply.m_header.m_flags |= WP_Flags::c_NonCritical;

    uint32_t pingReplyPayload[2] = {
        // public const uint c_Ping_Source_TinyCLR = 0x00000000;
        // public const uint c_Ping_Source_TinyBooter = 0x00000001;
        // public const uint c_Ping_Source_Host = 0x00000002;

        1, // Source TinyBooter

        // public const uint c_Ping_DbgFlag_Stop       = 0x00000001;
        // public const uint c_Ping_DbgFlag_BigEndian  = 0x02000002;
        // public const uint c_Ping_DbgFlag_AppExit    = 0x00000004;
        0, // Flags
    };

    // UNDONE: PrepareReply() - m_parent is NULL
    memcpy(reply.m_header.m_signature, MARKER_PACKET_V1,
           sizeof(reply.m_header.m_signature));
    reply.m_header.m_cmd = msg->m_header.m_cmd;
    reply.m_header.m_seq = m_lastOutboundMessage++;
    reply.m_header.m_seqReply = msg->m_header.m_seq;
    reply.m_header.m_flags |= WP_Flags::c_Reply;
    reply.m_header.m_size = sizeof(pingReplyPayload);
    reply.m_payload = (UINT8 *)pingReplyPayload;

    reply.m_header.m_crcData =
        SUPPORT_ComputeCRC(reply.m_payload, reply.m_header.m_size, 0);

    //
    // The CRC for the header is computed setting the CRC field to zero and then
    // running the CRC algorithm.
    //
    reply.m_header.m_crcHeader = 0;
    reply.m_header.m_crcHeader =
        SUPPORT_ComputeCRC((UINT8 *)&reply.m_header, sizeof(reply.m_header), 0);

    // tx header
    chSequentialStreamWrite((BaseSequentialStream *)&SDU1, (uint8_t *)&reply.m_header, sizeof(reply.m_header));

    // tx payload
    chSequentialStreamWrite((BaseSequentialStream *)&SDU1, (uint8_t *)reply.m_payload, reply.m_header.m_size);
  }
};


WP_Message wpMessage;

static Receiver rec(&wpMessage);
static BlinkerThread blinker1;

//  Application entry point.
int main(void) {

  //   HAL initialization, this also initializes the configured device drivers
  //      and performs the board-specific initializations.
  halInit();
  System::init();

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

  // start blinker thread
  blinker1.start(NORMALPRIO + 20);

  // start receiver thread
  rec.start(NORMALPRIO + 10);

  //  Normal main() thread activity it does nothing except sleeping in a loop 
  while (true) {
    BaseThread::sleep(MS2ST(1000));
  }
}
