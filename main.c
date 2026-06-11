#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define SERVO_PIN 15 

int main() {
    stdio_init_all();
    
    uint32_t clock_freq = clock_get_hz(clk_sys);
    
    // PWM setup
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);
    
    // PWM config
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 40.0f);
    pwm_config_set_wrap(&config, 62500 - 1);
    pwm_init(slice, &config, true);
    
    uint16_t stop_val = 4687;
    uint16_t cw_full  = 2187;
    uint16_t ccw_full = 7187;

    while (true) {
        pwm_set_chan_level(slice, channel, stop_val);
        sleep_ms(3000);

        pwm_set_chan_level(slice, channel, cw_full);
        sleep_ms(3000);

        pwm_set_chan_level(slice, channel, ccw_full);
        sleep_ms(3000);
    }
}
