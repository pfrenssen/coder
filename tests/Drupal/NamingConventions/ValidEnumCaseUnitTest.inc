<?php

enum Test: int {
  // Must not start with lower case.
  case one = 1;
  // Must not contain underscores.
  case TWO_TEST = 2;
  // Must not contain only upper case.
  case THREE = 3;
  // Upper case parts are not allowed.
  case FourJSONCase = 4;
  case FiveAndAHorseCorrect = 5;
}
