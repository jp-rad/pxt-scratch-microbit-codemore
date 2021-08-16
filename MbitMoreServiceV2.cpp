#include "pxt.h"

#include "MicroBit.h"
#include "MicroBitConfig.h"

#if MICROBIT_CODAL
#if CONFIG_ENABLED(DEVICE_BLE)

/**
  * Class definition for the Scratch MicroBit More Service.
  * Provides a BLE service to remotely controll Micro:bit from Scratch3.
  */
#include "MbitMoreServiceV2.h"

int gpio[] = {0, 1, 2, 8, 13, 14, 15, 16};
int analogIn[] = {0, 1, 2};
int digitalIn[] = {0, 1, 2}; // PullUp at connected to be same behaviour as the standard extension.

/**
 * Get voltage of the power supply [mV].
 */
int getPowerVoltage(void)
{
  // TODO:
}

/**
  * Constructor.
  * Create a representation of the MbitMoreService
  * @param _uBit The instance of a MicroBit runtime.
  */
MbitMoreService::MbitMoreService(MicroBit &_uBit)
    : uBit(_uBit)
{
  // TODO: Constructor
}

void MbitMoreService::initConfiguration()
{
  // TODO: Initialize pin configuration.
}

// /**
//   * Callback. Invoked when AnalogIn is read via BLE.
//   */
// void MbitMoreService::onReadAnalogIn(GattReadAuthCallbackParams *authParams)
// {
//   // TODO: Invoked when AnalogIn is read via BLE
// }

// /**
//   * Callback. Invoked when any of our attributes are written via BLE.
//   */
// void MbitMoreService::onDataWritten(const GattWriteCallbackParams *params)
// {
//   // TODO: Invoked when any of our attributes are written via BLE
// }

/**
 * Make it listen events of the event type on the pin.
 * Remove listener if the event type is MICROBIT_PIN_EVENT_NONE.
 */
void MbitMoreService::listenPinEventOn(int pinIndex, int eventType)
{
  // TODO: listenPinEventOn
}

/**
 * Callback. Invoked when a pin event sent.
 */
void MbitMoreService::onPinEvent(MicroBitEvent evt)
{
  // TODO: onPinEvent
}

/**
  * Button update callback
  */
void MbitMoreService::onButtonChanged(MicroBitEvent e)
{
  // TODO: Button update callback
}

void MbitMoreService::onGestureChanged(MicroBitEvent e)
{
  if (e.value == MICROBIT_ACCELEROMETER_EVT_SHAKE)
  {
    gesture = gesture | 1;
  }
  if (e.value == MICROBIT_ACCELEROMETER_EVT_FREEFALL)
  {
    gesture = gesture | 1 << 1;
  }
}

/**
 * Normalize angle in upside down.
 */
int MbitMoreService::normalizeCompassHeading(int heading)
{
  // TODO:  Normalize angle in upside down.
}

/**
 * Convert roll/pitch radians to Scratch extension value (-1000 to 1000).
 */
int MbitMoreService::convertToTilt(float radians)
{
  float degrees = (360.0f * radians) / (2.0f * PI);
  float tilt = degrees * 1.0f / 90.0f;
  if (degrees > 0)
  {
    if (tilt > 1.0f)
      tilt = 2.0f - tilt;
  }
  else
  {
    if (tilt < -1.0f)
      tilt = -2.0f - tilt;
  }
  return (int)(tilt * 1000.0f);
}

void MbitMoreService::updateGesture()
{
  // TODO:
}

void MbitMoreService::resetGesture()
{
  gesture = gesture & (1 << 2); // Save moved state to detect continuous movement.
}

void MbitMoreService::updateDigitalValues()
{
  // TODO: 
}

void MbitMoreService::updatePowerVoltage()
{
  powerVoltage = getPowerVoltage();
}

void MbitMoreService::updateAnalogValues()
{
  //
}

void MbitMoreService::updateLightSensor()
{
  if (lightSensingDuration <= 0)
  {
    uBit.display.setDisplayMode(DisplayMode::DISPLAY_MODE_BLACK_AND_WHITE);
    return;
  }
  lightLevel = uBit.display.readLightLevel();
  if (lightSensingDuration < 255) // over 255 means no-limited.
  {
    lightSensingDuration--;
  }
}

void MbitMoreService::updateAccelerometer()
{
  //
}

void MbitMoreService::updateMagnetometer()
{
  //
}

void MbitMoreService::setPullMode(int pinIndex, PinMode pull)
{
  //
}

void MbitMoreService::setDigitalValue(int pinIndex, int value)
{
  //
}

void MbitMoreService::setAnalogValue(int pinIndex, int value)
{
  //
}

void MbitMoreService::setServoValue(int pinIndex, int angle, int range, int center)
{
  //
}

void MbitMoreService::setPinModeTouch(int pinIndex)
{
  //
}

void MbitMoreService::setLightSensingDuration(int duration)
{
  lightSensingDuration = duration;
}

void MbitMoreService::composeDefaultData(uint8_t *buff)
{
  // Tilt value is sent as int16_t big-endian.
  int16_t tiltX = (int16_t)convertToTilt(rotation[1]);
  buff[0] = (tiltX >> 8) & 0xFF;
  buff[1] = tiltX & 0xFF;
  int16_t tiltY = (int16_t)convertToTilt(rotation[0]);
  buff[2] = (tiltY >> 8) & 0xFF;
  buff[3] = tiltY & 0xFF;
  buff[4] = (uint8_t)buttonAState;
  buff[5] = (uint8_t)buttonBState;
  buff[6] = (uint8_t)(((digitalValues >> 0) & 1) ^ 1);
  buff[7] = (uint8_t)(((digitalValues >> 1) & 1) ^ 1);
  buff[8] = (uint8_t)(((digitalValues >> 2) & 1) ^ 1);
  buff[9] = (uint8_t)gesture;
}

/**
  * Notify shared data to Scratch3
  */
void MbitMoreService::notifySharedData()
{
  // TODO:
}

/**
 * Notify default micro:bit data to Scratch.
 */
void MbitMoreService::notifyDefaultData()
{
  // TODO:
}

/**
  * Notify data to Scratch3
  */
void MbitMoreService::notify()
{
  // TODO: 
}

/**
 * Set value to shared data.
 * shared data (0, 1, 2, 3)
 */
void MbitMoreService::setSharedData(int index, int value)
{
  // value (-32768 to 32767) is sent as int16_t little-endian.
  int16_t data = (int16_t)value;
  sharedData[index] = data;
  notifySharedData();
}

/**
 * Get value of a shared data.
 * shared data (0, 1, 2, 3)
 */
int MbitMoreService::getSharedData(int index)
{
  return (int)(sharedData[index]);
}

void MbitMoreService::onBLEConnected(MicroBitEvent _e)
{
  //
}

void MbitMoreService::onBLEDisconnected(MicroBitEvent _e)
{
  lightSensingDuration = 0;
}

/**
 * Update sensors.
 */
void MbitMoreService::update()
{
  updateDigitalValues();
  writeIo();
  updateLightSensor();
  updateAccelerometer();
  updateMagnetometer();
  writeSensors();
}

/**
  * Write IO characteristics.
  */
void MbitMoreService::writeIo()
{
  // TODO:
}

/**
  * Write shared data characteristics.
  */
void MbitMoreService::writeSharedData()
{
  // TODO: Write shared data characteristics
}

/**
  * Write data of all sensors to the characteristic.
  */
void MbitMoreService::writeSensors()
{
  //
}

void MbitMoreService::displayFriendlyName()
{
  //
}

void MbitMoreService::assigneCallbackDisplayTextCommand(p_displayTextCommand func){
  //
}

const uint16_t MBIT_MORE_BASIC_SERVICE = 0xf005;
const uint8_t MBIT_MORE_BASIC_TX[] = {0x52, 0x61, 0xda, 0x01, 0xfa, 0x7e, 0x42, 0xab, 0x85, 0x0b, 0x7c, 0x80, 0x22, 0x00, 0x97, 0xcc};
const uint8_t MBIT_MORE_BASIC_RX[] = {0x52, 0x61, 0xda, 0x02, 0xfa, 0x7e, 0x42, 0xab, 0x85, 0x0b, 0x7c, 0x80, 0x22, 0x00, 0x97, 0xcc};

const uint8_t MBIT_MORE_SERVICE[] = {0xa6, 0x2d, 0x57, 0x4e, 0x1b, 0x34, 0x40, 0x92, 0x8d, 0xee, 0x41, 0x51, 0xf6, 0x3b, 0x28, 0x65};
const uint8_t MBIT_MORE_EVENT[] = {0xa6, 0x2d, 0x00, 0x01, 0x1b, 0x34, 0x40, 0x92, 0x8d, 0xee, 0x41, 0x51, 0xf6, 0x3b, 0x28, 0x65};
const uint8_t MBIT_MORE_IO[] = {0xa6, 0x2d, 0x00, 0x02, 0x1b, 0x34, 0x40, 0x92, 0x8d, 0xee, 0x41, 0x51, 0xf6, 0x3b, 0x28, 0x65};
const uint8_t MBIT_MORE_ANALOG_IN[] = {0xa6, 0x2d, 0x00, 0x03, 0x1b, 0x34, 0x40, 0x92, 0x8d, 0xee, 0x41, 0x51, 0xf6, 0x3b, 0x28, 0x65};
const uint8_t MBIT_MORE_SENSORS[] = {0xa6, 0x2d, 0x00, 0x04, 0x1b, 0x34, 0x40, 0x92, 0x8d, 0xee, 0x41, 0x51, 0xf6, 0x3b, 0x28, 0x65};
const uint8_t MBIT_MORE_SHARED_DATA[] = {0xa6, 0x2d, 0x00, 0x10, 0x1b, 0x34, 0x40, 0x92, 0x8d, 0xee, 0x41, 0x51, 0xf6, 0x3b, 0x28, 0x65};

#endif // #if CONFIG_ENABLED(DEVICE_BLE)
#endif // #if MICROBIT_CODAL
