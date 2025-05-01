#include "Melody.h"

const NonBlockingMelody::Note notes[4] = {

  {BUZZER_FREQUENCY_HERTZ, 50},
  {0, 5},
  {BUZZER_FREQUENCY_HERTZ + 880, 75},
  {0, 5}

};

Melody MELODY_COINCIDENCE_EVENT = {notes, 4, 1};
