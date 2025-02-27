#include "Watchdog.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Initialize everything
// ================================================================================================
void Watchdog::begin() {



}

// ================================================================================================
// Update the watchdog
// ================================================================================================
void Watchdog::update() {

  // Check if the available free heap or largest block size that can be allocated are below the allowed thresholds
  if (ESP.getFreeHeap() < WATCHDOG_MINIMUM_HEAP_BYTES || ESP.getMaxAllocHeap() < WATCHDOG_MINIMUM_HEAP_BLOCK_BYTES) {

    // OH SHIT, OH FUCK, OH SHIT, OH FUCK, NO MORE MEMORY REBOOT NOW!!!!
    ESP.restart();

  }

}