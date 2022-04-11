#include "gesture.h"

#define ALWAYS_CALIBRATING CALIBRATION_LOOPS == -1

ICommunication* comm;
int loops = 0;
void setup() {
  #if COMMUNICATION == COMM_SERIAL
    comm = new SerialCommunication();
  #elif COMMUNICATION == COMM_BTSERIAL
    comm = new BTSerialCommunication();
  #endif  
  comm->start();

  setupInputs();

  #if USING_FORCE_FEEDBACK
    setupServoHaptics();  
  #endif
  
}

void loop() {
  if (!comm->isOpen()){
    return;
  }
  
  #if USING_CALIB_PIN
  bool calibButton = getButton(PIN_CALIB) != INVERT_CALIB;
  if (calibButton)
    loops = 0;
  #else
  bool calibButton = false;
  #endif
  
  bool calibrate = false;
  if (loops < CALIBRATION_LOOPS || ALWAYS_CALIBRATING){
    calibrate = true;
    loops++;
  }
  
  int* fingerPos = getFingerPositions(calibrate, calibButton);
  bool joyButton = getButton(PIN_JOY_BTN) != INVERT_JOY;
  
  bool aButton = getButton(PIN_A_BTN) != INVERT_A;
  bool bButton = getButton(PIN_B_BTN) != INVERT_B;
  bool menuButton = getButton(PIN_MENU_BTN) != INVERT_MENU;

  Gestures gestures = detectGestures(fingerPos);
  
  comm->output(encode(fingerPos, getJoyX(), getJoyY(), joyButton, gestures.trigger, aButton, bButton, gestures.grab, gestures.pinch, calibButton, menuButton));

  #if USING_FORCE_FEEDBACK
    char received[100];
    if (comm->readData(received)){
      int hapticLimits[5];
      decodeData(received, hapticLimits);
      writeServoHaptics(hapticLimits);
    }
  #endif
  delay(LOOP_TIME);
}
