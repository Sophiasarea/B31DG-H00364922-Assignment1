#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// DEBUG
#ifdef DEBUG
  #define TIMING_FACTOR 1000
// PRODUCTION
#else
  #define TIMING_FACTOR 1
#endif

#define PB1_GPIO_NUM 1
#define PB2_GPIO_NUM 2
#define LED1_GPIO_NUM 18
#define LED2_GPIO_NUM 10
#define TON_1 900
#define TOFF 500
#define NUM_PULSES 17
#define IDLE_TIME 6500
#define TSYNC_ON 50

bool output_enabled = false;
bool output_selected = false;

void normal_data_waveform() {
    for (int i = 0; i < NUM_PULSES; i++) {
        // pulse on-time
        gpio_set_level(LED1_GPIO_NUM, 1);
        vTaskDelay((TON_1 + TSYNC_ON * (i - 1)) / portTICK_PERIOD_MS);

        // pulse off-time
        gpio_set_level(LED1_GPIO_NUM, 0);
        vTaskDelay(TOFF / portTICK_PERIOD_MS);
    }

    vTaskDelay(IDLE_TIME / portTICK_PERIOD_MS);
}

void alternative_data_waveform() {
    for (int i = NUM_PULSES; i > 0; i--) {
        // pulse on-time
        gpio_set_level(LED1_GPIO_NUM, 1);
        vTaskDelay((TON_1 + TSYNC_ON * (i - 1)) / portTICK_PERIOD_MS);

        // pulse off-time
        gpio_set_level(LED1_GPIO_NUM, 0);
        vTaskDelay(TOFF / portTICK_PERIOD_MS);
    }

    vTaskDelay(IDLE_TIME / portTICK_PERIOD_MS);
}

void sync_signal() {
    // turn on the SYNC pulse
    gpio_set_level(LED2_GPIO_NUM, 1);
    vTaskDelay(TSYNC_ON / portTICK_PERIOD_MS);

    // turn off
    gpio_set_level(LED2_GPIO_NUM, 0);
}

void app_main() {
    // Button
    gpio_set_direction(PB1_GPIO_NUM, GPIO_MODE_INPUT);
    gpio_set_direction(PB2_GPIO_NUM, GPIO_MODE_INPUT);

    // LED
    gpio_set_direction(LED1_GPIO_NUM, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED2_GPIO_NUM, GPIO_MODE_OUTPUT);

    while (1) {
        sync_signal();

        // Read the state of push buttons
        bool pb1_state = gpio_get_level(PB1_GPIO_NUM);
        bool pb2_state = gpio_get_level(PB2_GPIO_NUM);

        // output enable
        if (pb1_state == 1 && output_enabled == false) {
            output_enabled = true;
            gpio_set_level(LED1_GPIO_NUM, 1);
        } else if (pb1_state == 0 && output_enabled == true) {
            output_enabled = false;
            gpio_set_level(LED1_GPIO_NUM, 0);
        }

        // output select
        if (pb2_state == 1 && output_selected == false) {
            output_selected = true;
            alternative_data_waveform();
        } else if (pb2_state == 0 && output_selected == true) {
            output_selected = false;
            normal_data_waveform();
        }
    }
}
