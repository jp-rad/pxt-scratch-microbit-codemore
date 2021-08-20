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
  //
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
  //
}

void MbitMoreService::updateGesture()
{
  // TODO:
}

void MbitMoreService::resetGesture()
{
  //
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
  //
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
  //
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
  //
}

/**
 * Get value of a shared data.
 * shared data (0, 1, 2, 3)
 */
int MbitMoreService::getSharedData(int index)
{
  //
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


#endif // #if CONFIG_ENABLED(DEVICE_BLE)
#endif // #if MICROBIT_CODAL
