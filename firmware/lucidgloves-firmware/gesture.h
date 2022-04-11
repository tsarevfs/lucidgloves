struct Gestures {
  bool trigger = false;
  bool grab = false;
  bool pinch = false;
};

Gestures detectGestures(int* fingerPos);
