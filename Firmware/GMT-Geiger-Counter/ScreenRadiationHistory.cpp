#include "ScreenRadiationHistory.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenRadiationHistory::ScreenRadiationHistory():

  // Initialize members
  ScreenBasic(STRING_RADIATION_HISTORY_TITLE),
  _historyIndex(0),
  _historyTimerMilliseconds(0),
  _timeSteps(round(RADIATION_HISTORY_LENGTH_MINUTES / 4.0)),
  _countSteps(round(RADIATION_HISTORY_MINIMUM_SCALE_CPM / 4.0)),
  _average(  2, 213, 105, IMAGE_AVERAGE, "0"),
  _maximum(108, 213, 105, IMAGE_MAXIMUM, "0"),
  _minimum(214, 213, 104, IMAGE_MINIMUM, "0")

{

  // Set all elements in the history array to an impossibly high value marking them as invalid
  for (uint8_t sample = 0; sample < RADIATION_HISTORY_LENGTH_MINUTES; sample++) {

    _history[sample] = UINT16_MAX;

  }

}

// ================================================================================================
// Update
// ================================================================================================
void ScreenRadiationHistory::update(XPT2046::Point position) {

  // Update the basic screen
  ScreenBasic::update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenRadiationHistory::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw background
  canvas.fillRect(2, 31, 316, 181, COLOR_DARK_GRAY);

  // Draw grid
  canvas.drawFastVLine( 86,  42, 159, COLOR_LIGHT_GRAY);
  canvas.drawFastVLine(160,  42, 159, COLOR_LIGHT_GRAY);
  canvas.drawFastVLine(234,  42, 159, COLOR_LIGHT_GRAY);
  canvas.drawFastVLine(307,  42, 159, COLOR_LIGHT_GRAY);
  canvas.drawFastHLine( 13,  41, 295, COLOR_LIGHT_GRAY);
  canvas.drawFastHLine( 13,  81, 294, COLOR_LIGHT_GRAY);
  canvas.drawFastHLine( 13, 121, 294, COLOR_LIGHT_GRAY);
  canvas.drawFastHLine( 13, 161, 294, COLOR_LIGHT_GRAY);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_LIGHT_GRAY);

  // Draw time steps
  canvas.setCursor(236, 199);
  canvas.print("-" + String(_timeSteps * 1) + STRING_RADIATION_HISTORY_MINUTES_ABBREVIATION);
  canvas.setCursor(162, 199);
  canvas.print("-" + String(_timeSteps * 2) + STRING_RADIATION_HISTORY_MINUTES_ABBREVIATION);
  canvas.setCursor(88, 199);
  canvas.print("-" + String(_timeSteps * 3) + STRING_RADIATION_HISTORY_MINUTES_ABBREVIATION);
  canvas.setCursor(14, 199);
  canvas.print(String("-") + RADIATION_HISTORY_LENGTH_MINUTES + STRING_RADIATION_HISTORY_MINUTES_ABBREVIATION);

  // Draw counts steps
  canvas.setCursor(14, 177);
  canvas.print(String(_countSteps * 1));
  canvas.setCursor(14, 137);
  canvas.print(String(_countSteps * 2));
  canvas.setCursor(14, 97);
  canvas.print(String(_countSteps * 3));
  canvas.setCursor(14, 57);
  canvas.print(String(_countSteps * 4));

  // Calculate with and height of line elements
  double width  = 294.0 / (RADIATION_HISTORY_LENGTH_MINUTES - 1);
  double height = 159.0 / (_countSteps * 4); 

  // Get starting position of the first line element
  uint16_t x = 306;
  uint16_t y = 200 - round(height * _history[_historyIndex]); 

  // For all sample in the history array
  for (uint8_t sample = 1; sample < RADIATION_HISTORY_LENGTH_MINUTES; sample++) {

    // Calculate wrapped index from the current history index offset by the sample
    uint8_t wrappedIndex = (_historyIndex + RADIATION_HISTORY_LENGTH_MINUTES - sample) % RADIATION_HISTORY_LENGTH_MINUTES;

    // If the sample is not invalid
    if (_history[wrappedIndex] != UINT16_MAX) {

      // Calculate end position of line element
      uint16_t dx = 306 - round(width  * sample);
      uint16_t dy = 200 - round(height * _history[wrappedIndex]);

      // Draw line between the two positions
      canvas.drawLine(x, y, dx, dy, COLOR_MEDIUM_GREEN);

      // Set the next starting position to the current end position
      x = dx;
      y = dy;

    }

  }

  // Draw axis
  canvas.drawFastVLine(12,  41, 161, COLOR_WHITE);
  canvas.drawFastHLine(12, 201, 296, COLOR_WHITE);
  canvas.fillTriangle(  7,  49,  12,  41,  17,  49, COLOR_WHITE);
  canvas.fillTriangle(299, 196, 307, 201, 299, 206, COLOR_WHITE);

  // Draw screen elements
  _average.draw(canvas);
  _maximum.draw(canvas);
  _minimum.draw(canvas);

}

// ================================================================================================
// Update the radiation history
// ================================================================================================
void ScreenRadiationHistory::setRadiationHistory(double countsPerMinute) {

  // If a minute has passed since the last update
  // This is not super precise but good enough to update the graph screen
  // Alternatively a hardware time could be used
  if (millis() - _historyTimerMilliseconds >= 60000){

    // Update the timer immediately
    _historyTimerMilliseconds = millis();

    // Set the history index to the next element in the array
    _historyIndex = (_historyIndex + 1) % RADIATION_HISTORY_LENGTH_MINUTES;

    // Set the element to the current counts per minute value
    _history[_historyIndex] = countsPerMinute;

    // Reset count variables
    double   averageCountsPerMinute = 0.0;
    uint32_t maximumCountsPerMinute = 0;
    uint32_t minimumCountsPerMinute = UINT16_MAX;
    uint8_t  validSamples           = 0;

    // For all samples in the history array
    for (uint8_t sample = 0; sample < RADIATION_HISTORY_LENGTH_MINUTES; sample++) {

      // Only update count variables if not an invalid count value
      if (_history[sample] != UINT16_MAX) {

        // Add sample to the sum of a counts per minute values
        averageCountsPerMinute += _history[sample];

        // If this sample is larger than the previous maximum set the maximum to it
        if (_history[sample] > maximumCountsPerMinute) { maximumCountsPerMinute = _history[sample]; }
        
        // If this sample is smaller than the previous minimum set the minimum to it
        if (_history[sample] < minimumCountsPerMinute) { minimumCountsPerMinute = _history[sample]; }

        // Increase the number of valid samples
        validSamples++;

      }

    }

    // Calculate average and prevent division by 0
    averageCountsPerMinute = (validSamples > 0) ? round(averageCountsPerMinute / validSamples) : 0.0;

    // Calculate time steps
    _timeSteps = round(RADIATION_HISTORY_LENGTH_MINUTES / 4.0);

    // If the maximum counts per minute value is less than 90% of the minimum history scale
    if (maximumCountsPerMinute * 1.1 < RADIATION_HISTORY_MINIMUM_SCALE_CPM) {
      
      // Set the count step size to the minimum history scale
      _countSteps = round(RADIATION_HISTORY_MINIMUM_SCALE_CPM / 4.0);

    // If If the maximum counts per minute value is more then the minimum history scale
    } else {

      // Scale the history scale accordingly
      _countSteps = round((maximumCountsPerMinute * 1.1) / 4.0);

    }

    // Set the screen elements
    _average.setValue(String((uint32_t)(averageCountsPerMinute)));
    _maximum.setValue(String(maximumCountsPerMinute));
    _minimum.setValue(String(minimumCountsPerMinute));

  }

}