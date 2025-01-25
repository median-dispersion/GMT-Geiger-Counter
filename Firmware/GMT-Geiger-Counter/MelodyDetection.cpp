#include "Melody.h"

const NonBlockingMelody::Note notes[2] = {

  {BUZZER_FREQUENCY_HERTZ, 10},
  {0, 5}

};

Melody MELODY_DETECTION = {notes, 2, 1};