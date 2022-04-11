#include "AdvancedConfig.h"
#include "config.h"
#include "gesture.h"

bool grabGesture(int *flexion){
  return (flexion[PINKY_IND] + flexion[RING_IND] + flexion[MIDDLE_IND] + flexion[INDEX_IND]) / 4 <= ANALOG_MAX/2 ? 0:1;
}

bool pinchGesture(int *flexion){
  return (flexion[INDEX_IND] + flexion[THUMB_IND]) / 2 <= ANALOG_MAX/2 ? 0:1;
}

bool triggerGesture(int *flexion){
  return flexion[INDEX_IND]<=(ANALOG_MAX/2)?0:1;
}

Gestures detectGestures(int* fingerPos){
  Gestures gestures;
  #if TRIGGER_GESTURE
  gestures.trigger = triggerGesture(fingerPos);
  #else
  gestures.trigger = getButton(PIN_TRIG_BTN) != INVERT_TRIGGER;
  #endif

  #if GRAB_GESTURE
  gestures.grab = grabGesture(fingerPos);
  #else
  gestures.grab = getButton(PIN_GRAB_BTN) != INVERT_GRAB;
  #endif

  #if PINCH_GESTURE
  gestures.pinch = pinchGesture(fingerPos);
  #else
  gestures.pinch = getButton(PIN_PNCH_BTN) != INVERT_PINCH;
  #endif

  return gestures;
}
