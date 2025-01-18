<?php

enum Test: int {
  // Must not start with lower case.
  case one = 1;
  // Must not contain underscores.
  case TWO_TEST = 2;
  // Must not contain only upper case.
  case THREE = 3;
  // Upper case parts are allowed for now.
  case FourJSONCase = 4;
  case FiveAndAHorseCorrect = 5;
  case UIExample = 6;
}

// Those are all ok.
enum FiscalQuarter {
  case Q1;
  case Q2;
  case Q3;
  case Q4;
}

enum Plan {
  case A;
  case B;
  case C;
}
