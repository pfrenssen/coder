<?php

enum Test: int {
  // Must not start with lower case.
  case one = 1;
  // Must not contain underscores.
  case TWO_TEST = 2;
}
