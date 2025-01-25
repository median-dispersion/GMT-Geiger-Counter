#include "Melody.h"

const NonBlockingMelody::Note notes[8] = {
  
  {BUZZER_FREQUENCY_HERTZ, 50},
  {0, 50},
  {BUZZER_FREQUENCY_HERTZ + 440, 50},
  {0, 50},
  {BUZZER_FREQUENCY_HERTZ, 50},
  {0, 50},
  {BUZZER_FREQUENCY_HERTZ + 440, 50},
  {0, 550}
  
};

Melody MELODY_WARNING = {notes, 8, 2};