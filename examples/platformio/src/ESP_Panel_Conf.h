/**
 * Debug configurations
 *
 */
/* Set to 1 if assert on error. Otherwise print error message */
#define ESP_PANEL_CHECK_RESULT_ASSERT       (0)         // 0/1

/* Set to 1 if print log message for debug */
#define ESP_PANEL_ENABLE_LOG                (0)         // 0/1

/**
 * Touch driver configurations
 *
 */
/* Maximum point number */
#define ESP_PANEL_TOUCH_MAX_POINTS          (3)

/* Maximum button number */
#define ESP_PANEL_TOUCH_MAX_BUTTONS         (1)

/* Model configurations */
// XPT2046
#define ESP_PANEL_TOUCH_XPT2046_Z_THRESHOLD             (400)   // Minimum Z pressure threshold
/**
 * Enable Interrupt (PENIRQ) output, also called Full Power Mode.
 * Enable this to configure the XPT2046 to output low on the PENIRQ output if a touch is detected.
 * This mode uses more power when enabled. Note that this signal goes low normally when a read is active.
 */
#define ESP_PANEL_TOUCH_XPT2046_INTERRUPT_MODE          (0)     // 0/1
/**
 * Keep internal Vref enabled.
 * Enable this to keep the internal Vref enabled between conversions. This uses slightly more power,
 * but requires fewer transactions when reading the battery voltage, aux voltage and temperature.
 *
 */
#define ESP_PANEL_TOUCH_XPT2046_VREF_ON_MODE            (0)     // 0/1
/**
 * Convert touch coordinates to screen coordinates.
 * When this option is enabled the raw ADC values will be converted from 0-4096 to 0-{screen width} or 0-{screen height}.
 * When this option is disabled the process_coordinates method will need to be used to convert the raw ADC values into a
 * screen coordinate.
 *
 */
#define ESP_PANEL_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS   (1)     // 0/1
/**
 * Enable data structure locking.
 * By enabling this option the XPT2046 driver will lock the touch position data structures when reading values from the
 * XPT2046 and when reading position data via API.
 * WARNING: enabling this option may result in unintended crashes.
 *
 */
#define ESP_PANEL_TOUCH_XPT2046_ENABLE_LOCKING          (0)     // 0/1
