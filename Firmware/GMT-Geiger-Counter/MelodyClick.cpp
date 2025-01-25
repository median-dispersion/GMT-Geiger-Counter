#include "Melody.h"

const NonBlockingMelody::Note notes[2] = {
  
  {BUZZER_FREQUENCY_HERTZ, 50},
  {0, 5}
  
};

Melody MELODY_CLICK = {notes, 2, 1};