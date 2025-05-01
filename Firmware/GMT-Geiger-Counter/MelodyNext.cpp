#include "Melody.h"

const NonBlockingMelody::Note notes[4] = {
  
  {BUZZER_FREQUENCY_HERTZ, 50},
  {0, 25},
  {BUZZER_FREQUENCY_HERTZ + 440, 50},
  {0, 5}
  
};

Melody MELODY_NEXT = {notes, 4, 1};