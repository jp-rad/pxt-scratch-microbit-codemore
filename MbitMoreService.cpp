#include "pxt.h"

#if !MICROBIT_CODAL

/**
  * Class definition for the Scratch MicroBit More Service.
  * Provides a BLE service to remotely controll Micro:bit from Scratch3.
  */
#include "MbitMoreService.h"

int digitalIn[] = {0, 1, 2}; // PullUp at connected to be same behaviour as the standard extension.

/**
  * Constructor.
  * Create a representation of the MbitMoreService
  * @param _uBit The instance of a MicroBit runtime.
  */
MbitMoreService::MbitMoreService(MicroBit &_uBit)
    : uBit(_uBit)
{
  displayTextCommand = NULL;
  
  // Calibrate the compass before start bluetooth service.
  if (!uBit.compass.isCalibrated())
  {
    uBit.compass.calibrate();
  }

  // Create the data structures that represent each of our characteristics in Soft Device.
  GattCharacteristic txCharacteristic(
      MBIT_MORE_BASIC_TX,
      (uint8_t *)&txData,
      0,
      sizeof(txData),
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);

  GattCharacteristic rxCharacteristic(
      MBIT_MORE_BASIC_RX,
      (uint8_t *)&rxBuffer,
      0,
      sizeof(rxBuffer),
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);

  // Set default security requirements
  txCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
  rxCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  GattCharacteristic *characteristics[] = {&txCharacteristic, &rxCharacteristic};
  GattService service(
      MBIT_MORE_BASIC_SERVICE, characteristics,
      sizeof(characteristics) / sizeof(GattCharacteristic *));

  uBit.ble->addService(service);

  txCharacteristicHandle = txCharacteristic.getValueHandle();
  rxCharacteristicHandle = rxCharacteristic.getValueHandle();

  uBit.ble->gattServer().write(
      txCharacteristicHandle,
      (uint8_t *)&txData,
      sizeof(txData));

  // Advertise this service.
  const uint16_t uuid16_list[] = {MBIT_MORE_BASIC_SERVICE};
  uBit.ble->gap().accumulateAdvertisingPayload(GapAdvertisingData::INCOMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));

  // Setup callbacks for events.
  uBit.ble->onDataWritten(this, &MbitMoreService::onDataWritten);

  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, this, &MbitMoreService::onBLEConnected, MESSAGE_BUS_LISTENER_IMMEDIATE);
  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, this, &MbitMoreService::onBLEDisconnected, MESSAGE_BUS_LISTENER_IMMEDIATE);

  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, this, &MbitMoreService::onButtonChanged, MESSAGE_BUS_LISTENER_IMMEDIATE);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, this, &MbitMoreService::onButtonChanged, MESSAGE_BUS_LISTENER_IMMEDIATE);
  uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_EVT_ANY, this, &MbitMoreService::onGestureChanged, MESSAGE_BUS_LISTENER_IMMEDIATE);
}

void MbitMoreService::initConfiguration()
{
  // Initialize pin configuration.
  for (size_t i = 0; i < sizeof(digitalIn) / sizeof(digitalIn[0]); i++)
  {
    setPullMode(digitalIn[i], PinMode::PullUp);
  }
}

/**
  * Callback. Invoked when any of our attributes are written via BLE.
  */
void MbitMoreService::onDataWritten(const GattWriteCallbackParams *params)
{
  uint8_t *data = (uint8_t *)params->data;

  if (params->handle == rxCharacteristicHandle && params->len > 0)
  {
    if (data[0] == S3BLE_CMD_DISPLAY_TEXT)
    {
      char text[params->len];
      memcpy(text, &(data[1]), (params->len) - 1);
      text[(params->len) - 1] = '\0';
      ManagedString mstr(text);
      if ((NULL==displayTextCommand) || ('@'!=text[0]) || displayTextCommand(uBit, mstr)) {
        uBit.display.stopAnimation();        // Do not wait the end of current animation as same as the standard extension.
        uBit.display.scrollAsync(mstr, 120); // Interval is corresponding with the standard extension.
      }
    }
    else if (data[0] == S3BLE_CMD_DISPLAY_LED)
    {
      uBit.display.stopAnimation();
      for (int y = 1; y < params->len; y++)
      {
        for (int x = 0; x < 5; x++)
        {
          uBit.display.image.setPixelValue(x, y - 1, (data[y] & (0x01 << x)) ? 255 : 0);
        }
      }
    }
  }
}

/**
  * Button update callback
  */
void MbitMoreService::onButtonChanged(MicroBitEvent e)
{
  int state;
  if (e.value == MICROBIT_BUTTON_EVT_UP)
  {
    state = 0;
  }
  if (e.value == MICROBIT_BUTTON_EVT_DOWN)
  {
    state = 1;
  }
  if (e.value == MICROBIT_BUTTON_EVT_HOLD)
  {
    state = 5;
  }
  // if (e.value == MICROBIT_BUTTON_EVT_CLICK)
  // {
  //   state = 3;
  // }
  // if (e.value == MICROBIT_BUTTON_EVT_LONG_CLICK)
  // {
  //   state = 4;
  // }
  // if (e.value == MICROBIT_BUTTON_EVT_DOUBLE_CLICK)
  // {
  //   state = 6;
  // }
  if (e.source == MICROBIT_ID_BUTTON_A)
  {
    buttonAState = state;
  }
  if (e.source == MICROBIT_ID_BUTTON_B)
  {
    buttonBState = state;
  }
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
  int old[] = {lastAcc[0], lastAcc[1], lastAcc[2]};
  lastAcc[0] = uBit.accelerometer.getX();
  lastAcc[1] = uBit.accelerometer.getY();
  lastAcc[2] = uBit.accelerometer.getZ();
  if ((gesture >> 2) & 1)
  {
    gesture = gesture ^ (1 << 2);
    return;
  }
  int threshold = 50;
  if ((abs(lastAcc[0] - old[0]) > threshold) || (abs(lastAcc[1] - old[1]) > threshold) || (abs(lastAcc[2] - old[2]) > threshold))
  {
    // Moved
    gesture = gesture | (1 << 2);
  }
}

void MbitMoreService::resetGesture()
{
  gesture = gesture & (1 << 2); // Save moved state to detect continuous movement.
}

void MbitMoreService::updateDigitalValues()
{
  digitalValues = 0;
  for (size_t i = 0; i < sizeof(digitalIn) / sizeof(digitalIn[0]); i++)
  {
    if (uBit.io.pin[digitalIn[i]].isDigital())
    {
      if (uBit.io.pin[digitalIn[i]].isInput())
      {
        digitalValues =
            digitalValues | (uBit.io.pin[digitalIn[i]].getDigitalValue() << digitalIn[i]);
      }
    }
  }
}

void MbitMoreService::updateAccelerometer()
{
  rotation[0] = uBit.accelerometer.getPitchRadians();
  rotation[1] = uBit.accelerometer.getRollRadians();
}

void MbitMoreService::setPullMode(int pinIndex, PinMode pull)
{
  uBit.io.pin[pinIndex].getDigitalValue(pull);
  // @pullMode[pinIndex] = pull;
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
 * Notify default micro:bit data to Scratch.
 */
void MbitMoreService::notifyDefaultData()
{
  composeDefaultData(txData);
  uBit.ble->gattServer().notify(
      txCharacteristicHandle,
      (uint8_t *)&txData,
      sizeof(txData) / sizeof(txData[0]));
}

/**
  * Notify data to Scratch3
  */
void MbitMoreService::notify()
{
  if (uBit.ble->gap().getState().connected)
  {
    updateGesture();
    notifyDefaultData();
    resetGesture();
  }
  else
  {
    displayFriendlyName();
  }
}

void MbitMoreService::onBLEConnected(MicroBitEvent _e)
{
  uBit.display.stopAnimation(); // To stop display friendly name.
  initConfiguration();
  uBit.display.scrollAsync("v.0.5.2");
}

void MbitMoreService::onBLEDisconnected(MicroBitEvent _e)
{
  //
}

/**
 * Update sensors.
 */
void MbitMoreService::update()
{
  updateDigitalValues();
  updateAccelerometer();
}

void MbitMoreService::displayFriendlyName()
{
  ManagedString suffix(" MORE! ");
  uBit.display.scrollAsync(uBit.getName() + suffix, 120);
}

void MbitMoreService::assigneCallbackDisplayTextCommand(p_displayTextCommand func){
  displayTextCommand = func;
}

const uint16_t MBIT_MORE_BASIC_SERVICE = 0xf005;
const uint8_t MBIT_MORE_BASIC_TX[] = {0x52, 0x61, 0xda, 0x01, 0xfa, 0x7e, 0x42, 0xab, 0x85, 0x0b, 0x7c, 0x80, 0x22, 0x00, 0x97, 0xcc};
const uint8_t MBIT_MORE_BASIC_RX[] = {0x52, 0x61, 0xda, 0x02, 0xfa, 0x7e, 0x42, 0xab, 0x85, 0x0b, 0x7c, 0x80, 0x22, 0x00, 0x97, 0xcc};

#endif // #if !MICROBIT_CODAL
