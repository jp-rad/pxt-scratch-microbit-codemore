#ifndef SCRATCH_MORE_SERVICE_H
#define SCRATCH_MORE_SERVICE_H

#include "pxt.h"

#if !MICROBIT_CODAL

#include "MicroBit.h"
#include "MbitCodemore.h"

// UUIDs for our service and characteristics
extern const uint16_t MBIT_MORE_BASIC_SERVICE;
extern const uint8_t MBIT_MORE_BASIC_TX[];
extern const uint8_t MBIT_MORE_BASIC_RX[];

/**
  * Class definition for a MicroBitMore Service.
  * Provides a BLE service to remotely read the state of sensors from Scratch3.
  */
class MbitMoreService
{
public:
  /**
    * Constructor.
    * Create a representation of the MbitMoreService
    * @param _uBit The instance of a MicroBit runtime.
    */
  MbitMoreService(MicroBit &_uBit);

  void initConfiguration();

  /**
    * Notify data to Scratch3.
    */
  void notify();
  void notifyDefaultData();

  /**
   * Callback. Invoked when any of our attributes are written via BLE.
   */
  void onDataWritten(const GattWriteCallbackParams *params);

  /**
   * Invocked when the bluetooth connected.
   */
  void onBLEConnected(MicroBitEvent e);

  /**
   * Invocked when the bluetooth disconnected.
   */
  void onBLEDisconnected(MicroBitEvent e);

  void update();

  void updateDigitalValues();
  void updateAccelerometer();

private:

  // Sending data to Scratch3.
  uint8_t txData[20];

  // Recieving buffer from Scratch3.
  uint8_t rxBuffer[10];

  /**
   * Button state.
   */
  int buttonAState;
  int buttonBState;

  /**
   * Hold gesture state until next nofification.
   */
  int gesture;

  /**
   * Save the last accelerometer values to conpaire current for detecting moving.
   */
  int lastAcc[3];

  /**
   * Touch pin values.
   */
  uint32_t digitalValues;

  /**
   * Rotation value [pitch, roll] in radians.
   */
  float rotation[2];

  void setPullMode(int pinIndex, PinMode pull);
  
  void onButtonChanged(MicroBitEvent);
  void onGestureChanged(MicroBitEvent);

  void updateGesture(void);
  void resetGesture(void);

  int convertToTilt(float radians);

  void composeDefaultData(uint8_t *buff);

  void displayFriendlyName();

  // microbit runtime instance
  MicroBit &uBit;

  // Handles to access each characteristic when they are held by Soft Device.
  GattAttribute::Handle_t txCharacteristicHandle;
  GattAttribute::Handle_t rxCharacteristicHandle;

public:
  void assigneCallbackDisplayTextCommand(p_displayTextCommand displayTextCommand);

private:
  p_displayTextCommand displayTextCommand;

};

#endif // #if !MICROBIT_CODAL
#endif
