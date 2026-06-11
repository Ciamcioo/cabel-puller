#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define SERVO_PIN 15 
#define CW_BUT_PIN 13
#define CCW_BUT_PIN 12
#define STOP_BUT_PIN 11
//#define SLEEP_TIME 1000

uint slice = 0;
uint channel = 0;
uint16_t global_channel_level = 4687;

void initilize_pull_down_button(uint16_t button_pin);
void set_pwm_channel_level(uint16_t channel_level);
uint16_t get_channel_level_for_button_press();
void servo_stop();
void servo_start();

int main() {
    initilize_pull_down_button(CW_BUT_PIN);
    initilize_pull_down_button(CCW_BUT_PIN);
    initilize_pull_down_button(STOP_BUT_PIN);

    stdio_init_all();
    sleep_ms(2000);
    
    uint32_t clock_freq = clock_get_hz(clk_sys);
    
    // PWM setup
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(SERVO_PIN);
    channel = pwm_gpio_to_channel(SERVO_PIN);
    
    // PWM config
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 40.0f);
    pwm_config_set_wrap(&config, 62500 - 1);
    pwm_init(slice, &config, true);
    
    uint16_t stop_val = 4637; // Performed calibartion of the servo initial calcualted values cause the servo to twick a bit

    set_pwm_channel_level(stop_val);

    while (true) {
        if (gpio_get(STOP_BUT_PIN)) {
            set_pwm_channel_level(stop_val);
        } else {
            uint16_t channel_level = get_channel_level_for_button_press();
            set_pwm_channel_level(channel_level);
        }
    }
}

void initilize_pull_down_button(uint16_t button_pin) {
  gpio_init(button_pin);
  gpio_set_dir(button_pin, GPIO_IN);
  gpio_pull_down(button_pin);
  return;
}

void set_pwm_channel_level(uint16_t channel_level) {
  pwm_set_chan_level(slice, channel, channel_level);
  if (global_channel_level != channel_level) {
    printf("Channel level changed from '%d' to '%d'\n", global_channel_level, channel_level);
    global_channel_level = channel_level;
  }
  return;
}

uint16_t get_channel_level_for_button_press() {
  if (gpio_get(CW_BUT_PIN)) {
    return 2187;
  }
  else if (gpio_get(CCW_BUT_PIN)) {
    return 7187; 
  } 
  else {
    return 4637; // Performed calibartion of the servo initial calcualted values cause the servo to twick a bit
  }
}

void servo_stop() {
   pwm_set_enabled(slice, false);
   gpio_set_function(SERVO_PIN, GPIO_FUNC_SIO);
   gpio_set_dir(SERVO_PIN, GPIO_OUT);
   gpio_put(SERVO_PIN, 0);
}

void servo_start() {
   gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
   pwm_set_enabled(slice, true);
}


