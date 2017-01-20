# Wire Protocol

## Table of contents ##

- [Wire Protocol Message](#wire-protocol-message)
- [Data channels](#data-channels)
- [Receiving and transmitting data](#receiving-and-transmitting-data)
- [Receiver workflow](#receiver-workflow)
- [Wire Protocol Commands](#wire-protocol-commands)
- [How to add support for a new command](#how-to-add-support-for-a-new-command)
- [How to add support for new channels](#how-to-add-support-for-new-channels)
- [HAL interface](#hal-interface)


**About this document**

This document describes the Wire Protocol used by **nanoFramework** for debug and booter stage.
The protocol follows the implementation of the .NET Micro Framework Wire Protocol. The intention is to review it later in order to improve and simplify it.


# Wire Protocol Message

The message basic structure is comprised by:
- Signature which is basically a marker to detect the start of a new message packet. Has a fixed length.
- Header with several fields to cary packet sequence, flags, commands, CRC, etc. Has a fixed length.
  - CRC32 of header (for verification calculation this CRC32 field has to be zeroed).
  - CRC32 of payload, when it exists (for verification calculation this CRC32 field has to be zeroed).
  - Command code.
  - Sequence number of the message.
  - Sequence reply. Carries the sequence number of the message that the message is a reply to.
  - Flags.
  - Size of the payload.
- Payload for carrying data. Optional and its size is variable.

You can check the details on [WireProtocol.h](..\src\CLR\WireProtocol\WireProtocol.h)


# Data channels

Currently **nanoFramework** Wire Protocol supports only serial channels. The plan is to add support for USB (using CDC class device) and TCP.
To ease the port to new HAL/platforms the code is architecture so that only minimal changes are required to add support for new implementations. 


# Receiving and transmitting data

The code is architecture to receive and transmit data over a serial stream.
Preferably (and to use the reference implementation provided without much changes) the interface/API of the serial stream should:
- Allow checking if there is data available for reading.
- Allow reading sequentially (FIFO fashion) the input stream for a definite number of bytes. Having a timeout for the read operation is ideal to prevent bad/incomplete read operations.
- Allow writing to the transmit stream a definite number of bytes. Ideally in a non-blocking fashion to prevent bad/incomplete write operations.


# Receiver workflow

Follows a high-level description on how the Wire Protocol component works.

- RTOS thread - ```ReceiverThread(...)``` in [WireProtocol_ReceiverThread.c](..\src\CLR\WireProtocol\WireProtocol_ReceiverThread.c) - that loops continuously checking for available data in the receiving channel.
- On available data the reception of the message is initialized (WP_Message_Initialize) and prepared (WP_Message_PrepareReception) so the reception can actually occur and be processed by calling WP_Message_Process.
- During the reception states the input stream is read (```WP_ReceiveBytes(...)``` in [WireProtocol_HAL_Interface.c](..\src\CLR\WireProtocol\WireProtocol_HAL_Interface.c)) so the message header is received and it's integrity checked. Follows the reception and the integrity check of the payload, if there is any.
- After a successful reception of the header (and payload, if any) the _Process_ state machine in [WireProtocol_Message.c](..\src\CLR\WireProtocol\WireProtocol_Message.c)) reaches the ```ReceiveState_CompletePayload``` state and calls the ```ProcessPayload(...)``` function.
- Inside ```ProcessPayload(...)``` the lookup table for the commands that are implemented is searched and, if the command is found, the respective handler is called. According to the command its processing can require extra processing or gathering data. Invariably the handler execution end with a call to ```ReplyToCommand(...)``` where the reply is sent back to the host device.
- When executing ```ReplyToCommand(...)``` the output stream is written (```WP_TransmitMessage(...)``` in [WireProtocol_HAL_Interface.c](..\src\CLR\WireProtocol\WireProtocol_HAL_Interface.c)) with the reply message.


# Wire Protocol Commands

Processing a command is carried in a handler function. 
The collection of the commands that are implemented resides in ```c_Lookup_Request```. This lookup structure is basically an array with the command code along with a pointer to the respective handler.


# How to add support for a new command

New commands are to be added inside [WireProtocol_Commands.c](..\src\CLR\WireProtocol\WireProtocol_Commands.c).

Add the function declaration and any required structure and/or type definition in [WireProtocol_Commands.h](..\src\CLR\WireProtocol\WireProtocol_Commands.h).
The actual code for the command handler function (and any required helper functions or extra processing) goes into [WireProtocol_Commands.c](..\src\CLR\WireProtocol\WireProtocol_Commands.c).

To add the command to the collection of the supported commands un-comment or add the respective line in the ```c_Lookup_Request``` variable in [WireProtocol_Message.c](..\src\CLR\WireProtocol\WireProtocol_Message.c).
Because this declaration uses a macro to add the declaration of a command, make sure the existing naming pattern is _**strictly**_ followed.

To ease code portability from .NET Micro Framework code base and maintain an understandable implementation the naming has been maintained or minimally adapted from the original C++ code.
Try to follow this as much as possible when implementing new commands or porting the original C++ code to C.


# How to add support for new channels

Current Wire Protocol implementation has support for transmission over serial port (UART/USART) and serial over USB (USB CDC device class).
Support for TCP channel is planned at a latter stage.

When adding support for new channels the functions ```ReceiveBytes(...)``` and ```TransmitMessage(...)``` in [WireProtocol_HAL_Interface.c](..\src\CLR\WireProtocol\WireProtocol_HAL_Interface.c)) are the ones that need to be reworked.
On both the relevant part is that they read/write to a serial stream a specified number of bytes. Preferably non blocking calls with a timeout. Please read the comments inside of each of those functions for the details.
The last piece that needs to be adjusted is the code inside the ```ReceiverThread(...)``` which is the RTOS thread that is running the Wire Protocol component. That thread is basically a loop with a wait state were the checks for existing data to be read on the input stream. On data available the ```WP_Message_Process(...)``` function is called.


# HAL interface

The Wire Protocol requires the following functions in order to interface with the HAL.
Weak implementations of each function are part of the core code.

- ```WP_TransmitMessage(...)``` in [WireProtocol_HAL_Interface.c](..\src\CLR\WireProtocol\WireProtocol_HAL_Interface.c)
- ```WP_ReceiveBytes(...)``` in [WireProtocol_HAL_Interface.c](..\src\CLR\WireProtocol\WireProtocol_HAL_Interface.c)
- ```WP_CheckAvailableIncomingData(...)``` in [WireProtocol_HAL_Interface.c](..\src\CLR\WireProtocol\WireProtocol_HAL_Interface.c)

An implementation for ChibiOS (including its HAL) is provided as a reference. Please check it at [WireProtocol_HAL_Interface.c](..\src\RTOS\ChibiOS\WireProtocol_HAL_Interface.c).
 
When porting **nanoFramework** to another RTOS or HAL follow the reference implementation to ease the port work.