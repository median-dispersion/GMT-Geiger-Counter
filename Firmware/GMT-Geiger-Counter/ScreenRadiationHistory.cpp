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
  _maximumCountsPerMinute(32),
  _historyTimerMilliseconds(0),
  _average(  2, 213, 105, IMAGE_AVERAGE, "0"),
  _maximum(108, 213, 105, IMAGE_MAXIMUM, "0"),
  _minimum(214, 213, 104, IMAGE_MINIMUM, "0")

{}

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

  // Calculate time and counts steps
  uint8_t  timeSteps   = round(RADIATION_HISTORY_LENGTH_MINUTES / 4.0);
  uint32_t countsSteps = (_maximumCountsPerMinute > 0) ? round(_maximumCountsPerMinute / 4.0) : round(32.0 / 4.0); // Prevent div by 0

  // Draw time steps
  canvas.setCursor(236, 199);
  canvas.print("-" + String(timeSteps * 1) + STRING_RADIATION_HISTORY_MINUTES_ABBREVIATION);
  canvas.setCursor(162, 199);
  canvas.print("-" + String(timeSteps * 2) + STRING_RADIATION_HISTORY_MINUTES_ABBREVIATION);
  canvas.setCursor(88, 199);
  canvas.print("-" + String(timeSteps * 3) + STRING_RADIATION_HISTORY_MINUTES_ABBREVIATION);
  canvas.setCursor(14, 199);
  canvas.print(String("-") + RADIATION_HISTORY_LENGTH_MINUTES + STRING_RADIATION_HISTORY_MINUTES_ABBREVIATION);

  // Draw counts steps
  canvas.setCursor(14, 177);
  canvas.print(String(countsSteps * 1));
  canvas.setCursor(14, 137);
  canvas.print(String(countsSteps * 2));
  canvas.setCursor(14, 97);
  canvas.print(String(countsSteps * 3));
  canvas.setCursor(14, 57);
  canvas.print(String(_maximumCountsPerMinute));

  // Calculate with and height of line elements
  float width  = 294.0 / (RADIATION_HISTORY_LENGTH_MINUTES - 1);
  float height = (_maximumCountsPerMinute > 0) ? 159.0 / _maximumCountsPerMinute : 159.0 / 30.0; // Prevent div by 0

  // Get starting position of the first line element
  uint16_t x = 306;
  uint16_t y = 200 - round(height * _history[_historyIndex]); 

  // For all sample in the history array
  for (uint8_t sample = 1; sample < RADIATION_HISTORY_LENGTH_MINUTES; sample++) {

    // Calculate wrapped index from the current history index offset by the sample
    uint8_t wrappedIndex = (_historyIndex + RADIATION_HISTORY_LENGTH_MINUTES - sample) % RADIATION_HISTORY_LENGTH_MINUTES;

    // Calculate end position of line element
    uint16_t dx = 306 - round(width  * sample);
    uint16_t dy = 200 - round(height * _history[wrappedIndex]);

    // Draw line between the two positions
    canvas.drawLine(x, y, dx, dy, COLOR_MEDIUM_GREEN);

    // Set the next starting position to the current end position
    x = dx;
    y = dy;

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

    // Counts per minute average
    double countsPerMinuteAverage = 0.0;

    // Reset maximum counts per minute value
    _maximumCountsPerMinute = 0;

    // Minimum counts per minute value
    uint32_t _minimumCountsPerMinute = UINT16_MAX;

    // For all samples in the history array
    for (uint8_t sample = 0; sample < RADIATION_HISTORY_LENGTH_MINUTES; sample++) {

      // Add sample to the sum of a counts per minute values
      countsPerMinuteAverage += _history[sample];

      // If this sample is larger than the previous maximum set the maximum to it
      if (_history[sample] > _maximumCountsPerMinute) { _maximumCountsPerMinute = _history[sample]; }
      
      // If this sample is smaller than the previous minimum set the minimum to it
      if (_history[sample] < _minimumCountsPerMinute) { _minimumCountsPerMinute = _history[sample]; }

    }

    // Calculate average, prevent division by 0
    countsPerMinuteAverage = (countsPerMinuteAverage > 0) ? round(countsPerMinuteAverage / RADIATION_HISTORY_LENGTH_MINUTES) : 0.0;

    // Set the screen elements
    _average.setValue(String((uint32_t)(countsPerMinuteAverage)));
    _maximum.setValue(String(_maximumCountsPerMinute));
    _minimum.setValue(String(_minimumCountsPerMinute));

  }

}