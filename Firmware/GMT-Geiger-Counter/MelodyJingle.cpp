#include "Melody.h"

const NonBlockingMelody::Note notes[5] = {
  
  {BUZZER_FREQUENCY_HERTZ, 75},
  {BUZZER_FREQUENCY_HERTZ + 288, 125},
  {0, 100},
  {BUZZER_FREQUENCY_HERTZ + 611, 75},
  {BUZZER_FREQUENCY_HERTZ + 787, 75}
  
};

Melody MELODY_JINGLE = {notes, 5, 1};