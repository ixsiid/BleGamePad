#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define GAMEPAD_COUNT (1)
#include "BleGamePad.hpp"

#include <esp_log.h>

extern "C" void app_main();

void ble_send_task(void *arg) {
	gamepad_t pad = {
	    .buttons = 0,
	    .x	   = 0,
	    .y	   = 0,
	    .z	   = 0,
	    .rx	   = 0,
	    .ry	   = 0,
	    .rz	   = 0,
	    .slider  = 0x7fff,
	};

	while (true) {
		while (!BleGamePad.connected) vTaskDelay(1000 / portTICK_PERIOD_MS);
		vTaskDelay(2000 / portTICK_PERIOD_MS);

		while (BleGamePad.connected) {
			vTaskDelay(30 / portTICK_PERIOD_MS);

			pad.slider = (pad.slider + 1) & 0x7fff;
			BleGamePad.send(&pad);
		}
	}
}

void app_main() {
	vTaskDelay(10000 / portTICK_RATE_MS);
	TaskHandle_t hBLETask;
	xTaskCreatePinnedToCore(ble_send_task, "hid_task", 4 * 1048, nullptr, 5, &hBLETask, 0);
}
