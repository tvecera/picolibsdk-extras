#define NTP_SERVER "pool.ntp.org"
#define NTP_MSG_LEN 48
#define NTP_PORT 123
#define NTP_DELTA 2208988800 // seconds between 1 Jan 1900 and 1 Jan 1970
#define NTP_RESEND_TIME (10 * 1000)

#define WIFI_SSID     "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#include "pico/cyw43_arch.h"
#include "lwip/dns.h"

u8 key = 0;

typedef struct NTP_T_ {
		ip_addr_t ntp_server_address;
		bool dns_request_sent;
		struct udp_pcb *ntp_pcb;
		alarm_id_t ntp_resend_alarm;
		Bool start;
} NTP_T;

static void ntp_result(NTP_T *state, time_t *result) {
	if (result) {
		struct tm *utc = gmtime(result);
		printf("\nNTP odpoved: %02d/%02d/%04d %02d:%02d:%02d UTC\n", utc->tm_mday, utc->tm_mon + 1, utc->tm_year + 1900,
					 utc->tm_hour, utc->tm_min, utc->tm_sec);
	}

	if (state->ntp_resend_alarm > 0) {
		cancel_alarm(state->ntp_resend_alarm);
		state->ntp_resend_alarm = 0;
	}
	state->dns_request_sent = False;
	state->start = True;
}

static s64 ntp_failed_handler(alarm_id_t id, void *user_data);

static void ntp_request(NTP_T *state) {
	cyw43_arch_lwip_begin();
	struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, NTP_MSG_LEN, PBUF_RAM);
	auto *req = (u8 *) p->payload;
	memset(req, 0, NTP_MSG_LEN);
	req[0] = 0x1b;
	udp_sendto(state->ntp_pcb, p, &state->ntp_server_address, NTP_PORT);
	pbuf_free(p);
	cyw43_arch_lwip_end();
}

static s64 ntp_failed_handler(alarm_id_t id, void *user_data) {
	(void) id;

	auto *state = (NTP_T *) user_data;
	printf("NTP request failed!!!\n");
	ntp_result(state, nullptr);

	return 0;
}

static void ntp_dns_found(const char *hostname, const ip_addr_t *ipaddr, void *arg) {
	auto *state = (NTP_T *) arg;
	if (ipaddr) {
		state->ntp_server_address = *ipaddr;
		printf("IPv4 adresa NTP %s\n", ipaddr_ntoa(ipaddr));
		ntp_request(state);
	} else {
		printf("Nepovedlo se nacist DNS zaznam pro %s!!!\n", hostname);
		ntp_result(state, nullptr);
	}
}

static void ntp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16 port) {
	(void) pcb;

	auto *state = (NTP_T *) arg;
	u8 mode = pbuf_get_at(p, 0) & 0x7;
	u8 stratum = pbuf_get_at(p, 1);

	if (ip_addr_cmp(addr, &state->ntp_server_address) && port == NTP_PORT && p->tot_len == NTP_MSG_LEN &&
			mode == 0x4 && stratum != 0) {
		u8 seconds_buf[4] = {0};
		pbuf_copy_partial(p, seconds_buf, sizeof(seconds_buf), 40);
		u32 seconds_since_1900 = seconds_buf[0] << 24 | seconds_buf[1] << 16 | seconds_buf[2] << 8 | seconds_buf[3];
		u32 seconds_since_1970 = seconds_since_1900 - NTP_DELTA;
		time_t epoch = seconds_since_1970;
		ntp_result(state, &epoch);
	} else {
		printf("Nevalidni odpoved z NTP!!!\n");
		ntp_result(state, nullptr);
	}
	pbuf_free(p);
}

static NTP_T *ntp_init() {
	auto *state = (NTP_T *) calloc(1, sizeof(NTP_T));
	if (!state) {
		return nullptr;
	}
	state->ntp_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
	if (!state->ntp_pcb) {
		free(state);
		return nullptr;
	}
	udp_recv(state->ntp_pcb, ntp_recv, state);
	state->start = True;

	return state;
}

int main() {
	key = KeyGet();

	if (cyw43_arch_init()) {
		printf("Nepovedlo se nastartovat WIFI stack!!!\n");
		while (KeyGet() == NOKEY) {}
		ResetToBootLoader();
	}

	cyw43_arch_enable_sta_mode();

	if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
		printf("Nepovedlo se pripojit k WIFI siti %s!!!\n", WIFI_SSID);
		while (KeyGet() == NOKEY) {}
		ResetToBootLoader();
	}

	NTP_T *state = ntp_init();
	if (!state) {
		printf("Chyba pri inicializaci NTP stavu!!!\n");
		while (KeyGet() == NOKEY) {}
		ResetToBootLoader();
	}

	printf("IP adresa Picopadu: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));

	while (key != KEY_Y) {
		key = KeyGet();

		if (state && key == KEY_A && !state->dns_request_sent) {
			state->ntp_resend_alarm = add_alarm_in_ms(NTP_RESEND_TIME, ntp_failed_handler, state, True);
			cyw43_arch_lwip_begin();
			s8 err = dns_gethostbyname(NTP_SERVER, &state->ntp_server_address, ntp_dns_found, state);
			cyw43_arch_lwip_end();

			state->dns_request_sent = True;
			if (err == ERR_OK) {
				ntp_request(state);
			} else if (err != ERR_INPROGRESS) {
				printf("Nepovedlo se nacist DNS zaznam pro %s!!!\n", NTP_SERVER);
				ntp_result(state, nullptr);
			}
		}

		if (state && state->start) {
			printf("\nA: get time, X: screenshot, Y: exit\n");
			state->start = False;
		}

		if (key == KEY_X) {
#if USE_SCREENSHOT    // use screen shots
			ScreenShot();
#endif
		}
	}

	free(state);
	cyw43_arch_deinit();
	ResetToBootLoader();
}