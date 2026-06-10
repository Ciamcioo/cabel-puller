#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define SERVO_PIN 15 

int main() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    stdio_init_all();
    
    // Get the actual system clock frequency (default 125 MHz)
    uint32_t clock_freq = clock_get_hz(clk_sys);
    
    // PWM setup
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);
    
    // Configure PWM for 50 Hz servo signal
    // With 125 MHz clock, wrap=62500, divider=40: freq = 125M / (62500 * 40) = 50 Hz
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 40.0f);
    pwm_config_set_wrap(&config, 62500 - 1);
    pwm_init(slice, &config, true);
    
    // Servo pulse calculations for wrap=62500:
    // 1500μs (stop)     = 62500 * 1500 / 20000 = 4687
    // 700μs  (CW full)  = 62500 * 700  / 20000 = 2187
    // 2300μs (CCW full) = 62500 * 2300 / 20000 = 7187
    
    uint16_t stop_val = 4687;
    uint16_t cw_full  = 2187;
    uint16_t ccw_full = 7187;

    while (true) {
        // Stop
        pwm_set_chan_level(slice, channel, stop_val);
        sleep_ms(3000);

        // Full speed clockwise
        pwm_set_chan_level(slice, channel, cw_full);
        sleep_ms(3000);

        // Full speed counter-clockwise
        pwm_set_chan_level(slice, channel, ccw_full);
        sleep_ms(3000);
    }
}
