#include "Melody.h"

const NonBlockingMelody::Note notes[5] = {
  
  {BUZZER_FREQUENCY_HERTZ, 50},
  {0, 200},
  {BUZZER_FREQUENCY_HERTZ, 50},
  {0, 25},
  {BUZZER_FREQUENCY_HERTZ + 611, 200}
  
};

Melody MELODY_SUCCESS = {notes, 5, 1};