#include "pico/cyw43_arch.h"

u8 key = 0;

static int scan_result(void *env, const cyw43_ev_scan_result_t *result) {
	(void) env;

	if (result) {
		printf("ssid: %-32s\n", result->ssid);
		printf("RSSI: %4d CHAN: %3d MAC: %02x:%02x:%02x:%02x:%02x:%02x SEC: %u\n", result->rssi, result->channel,
					 result->bssid[0], result->bssid[1], result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5],
					 result->auth_mode);
	}
	return 0;
}

int main() {
	bool scan_in_progress = True;
	key = KeyGet();

	if (cyw43_arch_init()) {
		printf("Nepovedlo se nastartovat WIFI stack\n");
		while (KeyGet() == NOKEY) {}
		ResetToBootLoader();
	}

	cyw43_arch_enable_sta_mode();

	while (key != KEY_Y) {
		key = KeyGet();

		if (key == KEY_A && !scan_in_progress) {
			cyw43_wifi_scan_options_t scan_options = {0};
			int err = cyw43_wifi_scan(&cyw43_state, &scan_options, nullptr, scan_result);
			if (err == 0) {
				scan_in_progress = True;
			} else {
				printf("Chyba wifi scanu: %d\n", err);
			}
		}

		if (scan_in_progress && !cyw43_wifi_scan_active(&cyw43_state)) {
			printf("A: start scan, X: screenshot\n");
			scan_in_progress = False;
		}

		if (key == KEY_X) {
#if USE_SCREENSHOT    // use screen shots
			ScreenShot();
#endif
		}
	}
	cyw43_arch_deinit();
	ResetToBootLoader();
}