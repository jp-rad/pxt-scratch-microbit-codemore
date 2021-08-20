#ifndef SCRATCH_MORE_SERVICE_V2_H
#define SCRATCH_MORE_SERVICE_V2_H

#include "pxt.h"

#include "MicroBit.h"
#include "MicroBitConfig.h"

#if MICROBIT_CODAL
#if CONFIG_ENABLED(DEVICE_BLE)

#include "MicroBit.h"
#include "MbitCodemore.h"

#define SCRATCH_MORE_ID 2000

#define SCRATCH_MORE_EVT_NOTIFY 1

/**
 * Position of data format in a value holder.
 */
#define DATA_FORMAT_INDEX 19

// UUIDs for our service and characteristics
extern const uint16_t MBIT_MORE_BASIC_SERVICE;
extern const uint8_t MBIT_MORE_BASIC_TX[];
extern const uint8_t MBIT_MORE_BASIC_RX[];
extern const uint8_t MBIT_MORE_SERVICE[];
extern const uint8_t MBIT_MORE_EVENT[];
extern const uint8_t MBIT_MORE_IO[];
extern const uint8_t MBIT_MORE_SENSORS[];
extern const uint8_t MBIT_MORE_SHARED_DATA[];
extern const uint8_t MBIT_MORE_ANALOG_IN[];

/**
  * Class definition for a MicroBitMore Service.
  * Provides a BLE service to remotely read the state of sensors from Scratch3.
  */
class MbitMoreService
{
public:
  /**
    * Constructor.
    * Create a representation of the MbitMoreServiceV2
    * @param _uBit The instance of a MicroBit runtime.
    */
  MbitMoreService(MicroBit &_uBit);

  void initConfiguration();

  /**
    * Notify data to Scratch3.
    */
  void notify();
  void notifyDefaultData();
  void notifySharedData();

  /**
   * Invocked when the bluetooth connected.
   */
  void onBLEConnected(MicroBitEvent e);

  /**
   * Invocked when the bluetooth disconnected.
   */
  void onBLEDisconnected(MicroBitEvent e);

  /**
   * Callback. Invoked when a pin event sent.
   */
  void onPinEvent(MicroBitEvent evt);

  void update();

  void updateDigitalValues();
  void updatePowerVoltage();
  void updateAnalogValues();
  void updateLightSensor();
  void updateAccelerometer();
  void updateMagnetometer();

  void writeIo();
  void writeAnalogIn();
  void writeSensors();
  void writeSharedData();

private:

  void listenPinEventOn(int pinIndex, int eventType);
  void setPullMode(int pinIndex, PinMode pull);
  void setDigitalValue(int pinIndex, int value);
  void setAnalogValue(int pinIndex, int value);
  void setServoValue(int pinIndex, int angle, int range, int center);
  void setPinModeTouch(int pinIndex);
  void setLightSensingDuration(int duration);

  void onButtonChanged(MicroBitEvent);
  void onGestureChanged(MicroBitEvent);

  void updateGesture(void);
  void resetGesture(void);

  int normalizeCompassHeading(int heading);
  int convertToTilt(float radians);

  void composeDefaultData(uint8_t *buff);

  void displayFriendlyName();

  // microbit runtime instance
  MicroBit &uBit;

public:
  void assigneCallbackDisplayTextCommand(p_displayTextCommand displayTextCommand);

};

#endif // #if CONFIG_ENABLED(DEVICE_BLE)
#endif // #if MICROBIT_CODAL
#endif