// The main configuration file for the GMT-Geiger-Counter firmware

// Baud rate for serial communication 
#define SERIAL_BAUD_RATE 115200

// ================================================================================================
// Geiger Counter / Tube Settings
// ================================================================================================

// Enable pulse counting on a specific tube pin header
#define ENABLE_MAIN_TUBE     1
#define ENABLE_FOLLOWER_TUBE 0

// The total number of tubes connected to the motherboard (both main and follower)
#define NUMBER_OF_TUBES 1

// Conversion factor to convert counts per minute to micro severs per hour
// This is different for each type of tube and must be read off the datasheet
// SMB-20: 0.008095385
#define CONVERSION_FACTOR_CPM_TO_USVH 0.008095385

// CURRENTLY NOT USED, PLEASE IGNORE FOR NOW!
// The average length of a pulse in microseconds
// This is not the tube's dead time, but the length of the pulse the tube driver board generates
// To get an accurate value for a specific tube type, use the Calculate-Average-Pulse-Length firmware to measure it
// SBM-20: 115 µs
#define PULSE_LENGTH_MICROSECONDS 115

// Noise threshold, the minimum length in microseconds a pulse must be to count as an actual pulse
// Anything shorter than that will be ignored as noise
// This should be around 30% of the pulse length in microseconds
#define NOISE_THRESHOLD_MICROSECONDS 30

// Size of the moving average array
// The moving average array stores samples of counts per second 
// The size of the array defines how many seconds are being kept tack of
// A value of 60 (seconds) is recommended
#define MOVING_AVERAGE_SIZE 60

// The default integration time the Geiger counter starts at
// The integration time is the number of seconds the Geiger counter will average over to calculate the CPM value
// A shorter integration time give more accurate values for high levels of radiation, a lower in integration time for lower levels
#define DEFAULT_INTEGRATION_TIME 30

// ================================================================================================
// Buzzer settings
// ================================================================================================

// Enable the buzzer
#define ENABLE_BUZZER 1

// The buzzers base frequency in Hertz
// This should be provided by the buzzer manufacture
// Most buzzers have a base frequency of around 2000 Hertz
#define BUZZER_FREQUENCY_HERTZ 2400

// The value in µSv/h at which the warning sound is played
#define BUZZER_WARNING_LEVEL_USVH 2

// The value in µS/h at which the alarm sound is played
#define BUZZER_ALARM_LEVEL_USVH 10

// ================================================================================================
// Pin assignments
// ================================================================================================

// Pin assignments (these should not be changed!)
#define MOSI_PIN            11
#define MISO_PIN            13
#define SCK_PIN             12
#define DISPLAY_LED_PIN     9
#define DISPLAY_RST_PIN     8
#define DISPLAY_DC_PIN      3
#define DISPLAY_CS_PIN      18
#define TOUCH_CS_PIN        10
#define TOUCH_IRQ_PIN       14
#define BUZZER_PIN          47
#define SD_CS_PIN           17
#define MAIN_TRG_PIN        42
#define FOLLOWER_TRG_PIN    41
#define COINCIDENCE_TRG_PIN 40