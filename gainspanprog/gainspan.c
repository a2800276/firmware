#include "gainspan.h"

#define GS_SSP_HW SSP0_HW
#define GS_SSEL_PIN 0x0306
#define GS_SCK_PIN 0x0303
#define GS_MISO_PIN 0x0307
#define GS_MOSI_PIN 0x0308
#define GS_PROG_PIN P3_5_AS_GPIO_1_15
#define GS_nRESET_PIN P3_4_AS_GPIO_1_14
#define GS_MANUAL_SSEL_PIN P3_6_AS_GPIO_0_6
#define GS_DATA_AVAIL_PIN P1_9_AS_GPIO_1_2

#define GS_WIFI_FLASH_BASE 0x03000000
#define GS_WIFI_FLASH_SIZE 0x20000

#define GS_SHRAM_BASE 0x04000000
#define GS_SHRAM_SIZE 0x20000

#define GS_APP_FLASH0_BASE 0x06000000
#define GS_APP_FLASH0_SIZE 0x20000

#define GS_APP_FLASH1_BASE 0x08000000
#define GS_APP_FLASH1_SIZE 0x20000

#define GS_WLAN_FLASH_CONTROLLER_BASE 0x05000D00
#define GS_WLAN_FLASH_CONTROLLER_SIZE 0x100

#define GS_APP_FLASH0_CONTROLLER_BASE 0x07000300
#define GS_APP_FLASH0_CONTROLLER_SIZE 0x100

#define GS_APP_FLASH1_CONTROLLER_BASE 0x07000400
#define GS_APP_FLASH1_CONTROLLER_SIZE 0x100

#define GS_MAC_BASE 0x0801e800
#define GS_MAC_SIZE 8

#define GS_MAX_DUMP_SIZE_BYTES 32

#define GS_SPI_MODE 0
#define GS_LSB_FIRST true
#define GS_MANUAL_SSEL 1

typedef struct {
	uint8_t cmdClass;
	uint8_t reserved;
	uint8_t addInfo[2];
	uint8_t length[2];
} __attribute__((packed)) GS_HI_HEADER;

typedef enum {
	GS_DATA_SERVICE = 0,
	GS_KERNEL_MESSAGE_SERVICE = 1,
	GS_DOWNLOAD_SERVICE = 2,
	GS_DEBUG_SERVICE = 3,
	GS_TRACE_SERVICE = 6,
	GS_ROM_DEBUG_SERVICE = 7
} GS_SERVICE_CLASS;

typedef enum {
	GS_ROM_DEBUG_READ_REQUEST = 0x01,
	GS_ROM_DEBUG_WRITE_REQUEST = 0x02,
	GS_ROM_DEBUG_RMW_REQUEST = 0x03,
	GS_ROM_DEBUG_DUMP_REQUEST = 0x04,
	GS_ROM_DEBUG_READ_RESPONSE = 0x11,
	GS_ROM_DEBUG_WRITE_RESPONSE = 0x12,
	GS_ROM_DEBUG_RMW_RESPONSE = 0x13,
	GS_ROM_DEBUG_DUMP_RESPONSE = 0x14
} GS_ROM_DEBUG_CMD;

typedef struct {
	uint8_t req;
	uint8_t addr[4];
	uint8_t size_bits;
} __attribute__((packed)) GS_ROM_DEBUG_READ_REQUEST_STRUCT;

typedef struct {
	uint8_t resp;
	uint8_t size_bits;
	uint8_t data[4];
} __attribute__((packed)) GS_ROM_DEBUG_READ_RESPONSE_STRUCT;

typedef struct {
	uint8_t req;
	uint8_t addr[4];
	uint8_t reserved;
	uint8_t size_words[4];
} __attribute__((packed)) GS_ROM_DEBUG_DUMP_REQUEST_STRUCT;

typedef struct {
	uint8_t resp;
	uint8_t size_bits;
	uint8_t data[GS_MAX_DUMP_SIZE_BYTES];
} __attribute__((packed)) GS_ROM_DEBUG_DUMP_RESPONSE_STRUCT;

static uint8_t gs_seq_nbr;

void blink(uint8_t code) {
	while(1) {
		int i;
		for (i=0; i<10; i++) {
			int duration = 0;
			if (i<8) {
				duration = (code & (0x80 >> i)) ? 7 : 1;
			}
			if (duration > 0) write_pin(P1_17_AS_GPIO_0_12,true);
			delay(MAIN_CLOCK_MHZ * 5000 * duration);
			write_pin(P1_17_AS_GPIO_0_12,false);
			delay(MAIN_CLOCK_MHZ * 5000 * 10-duration);
		}
	}
}

bool gs_init(void) {
	//Init 1: pull SSEL up, set prog mode to GPIO27
	set_digital_output(GS_MANUAL_SSEL_PIN);
	write_pin(GS_MANUAL_SSEL_PIN, true);
	set_digital_output(GS_PROG_PIN);
	write_pin(GS_PROG_PIN, false);
	set_digital_input(GS_nRESET_PIN, true, false, false);	

#if (GS_MANUAL_SSEL)
	ssp_init(GS_SSP_HW, SSP_FRAMEFORMAT_SPI, GS_SPI_MODE, true, 8, MAIN_CLOCK_MHZ*4, 0xffff, GS_SCK_PIN, GS_MISO_PIN, GS_MOSI_PIN);
#else
	ssp_init(GS_SSP_HW, SSP_FRAMEFORMAT_SPI, GS_SPI_MODE, true, 8, MAIN_CLOCK_MHZ*4, GS_SSEL_PIN, GS_SCK_PIN, GS_MISO_PIN, GS_MOSI_PIN);
#endif
	gs_seq_nbr = 0;
	return true;
}

bool gs_reset(bool prog_mode) {
	write_pin(GS_PROG_PIN, prog_mode);
	delay(MAIN_CLOCK_MHZ * 10000);

	//Init 2: pull reset low
	write_pin(GS_nRESET_PIN, false);
	set_digital_output(GS_nRESET_PIN);	
	write_pin(GS_nRESET_PIN, false);
	delay(MAIN_CLOCK_MHZ * 10000);

	//Init 3: release reset (don't actively drive high, just a pullup)
	set_digital_input(GS_nRESET_PIN, true, false, false);	
	delay(MAIN_CLOCK_MHZ * 10000);
	return true;
}

void gs_parse_byte(uint8_t val) {
	if (val) blink(val);
}

void gs_parse_incoming(uint8_t* buf, uint16_t len) {
	uint16_t i;
	for (i=0; i < len; i++) gs_parse_byte(buf[i]);
}

bool gs_send_hi_command(GS_SERVICE_CLASS cmdClass, uint16_t addInfo, uint16_t body1Len, uint8_t* body1, uint16_t body2Len, uint8_t* body2) {
#if (GS_MANUAL_SSEL)
	write_pin(GS_MANUAL_SSEL_PIN, false);
#endif
	uint16_t bodyLen = body1Len + body2Len;
	uint8_t sof = 0xa5;
	GS_HI_HEADER header;
	header.cmdClass = cmdClass;
	header.reserved = 0;
	header.addInfo[0] = addInfo & 0xff;
	header.addInfo[1] = (addInfo>>8) & 0xff;
	header.length[0] = bodyLen & 0xff;
	header.length[1] = (bodyLen>>8) & 0xff;
	uint8_t sum = header.cmdClass + header.reserved
				+ header.addInfo[0] + header.addInfo[1]
				+ header.length[0] + header.length[1];
	uint8_t checksum = ~sum;
	uint8_t rcv1;
	uint8_t rcvHeader[sizeof(GS_HI_HEADER)];
	uint8_t rcvBody1[body1Len];
	uint8_t rcvBody2[body2Len];
	bool ok = true;
	if (ok) ok = ssp_write_read_sync8(GS_SSP_HW, 1, &sof, &rcv1, GS_LSB_FIRST);
	if (ok) gs_parse_incoming(&rcv1, 1);	
	if (ok) ok = ssp_write_read_sync8(GS_SSP_HW, sizeof(GS_HI_HEADER), (uint8_t*)(&header), rcvHeader, GS_LSB_FIRST);
	if (ok) gs_parse_incoming((uint8_t*)(&rcvHeader), sizeof(GS_HI_HEADER));	
	if (ok) ok = ssp_write_read_sync8(GS_SSP_HW, 1, &checksum, &rcv1, GS_LSB_FIRST);
	if (ok) gs_parse_incoming(&rcv1, 1);	
	if (body1Len > 0) {
		if (ok) ok = ssp_write_read_sync8(GS_SSP_HW, body1Len, body1, rcvBody1, GS_LSB_FIRST);
		if (ok) gs_parse_incoming(rcvBody1, body1Len);	
	}
	if (body2Len > 0) {
		if (ok) ok = ssp_write_read_sync8(GS_SSP_HW, body2Len, body2, rcvBody2, GS_LSB_FIRST);
		if (ok) gs_parse_incoming(rcvBody2, body2Len);	
	}
#if (GS_MANUAL_SSEL)
	write_pin(GS_MANUAL_SSEL_PIN, true);
#endif
	delay(MAIN_CLOCK_MHZ * 1000);
	return ok;
}

bool gs_send_read_cmd(uint32_t addr, uint8_t len_bytes, uint32_t* ret) {
	GS_ROM_DEBUG_READ_REQUEST_STRUCT body;
	body.req = GS_ROM_DEBUG_READ_REQUEST;
	body.addr[0] = addr & 0xff;
	body.addr[1] = (addr >> 8) & 0xff;
	body.addr[2] = (addr >> 16) & 0xff;
	body.addr[3] = (addr >> 24) & 0xff;
	body.size_bits = len_bytes * 8;
	bool ok = gs_send_hi_command(GS_ROM_DEBUG_SERVICE, 0, sizeof(GS_ROM_DEBUG_READ_REQUEST_STRUCT), (uint8_t*)(&body), 0, NULL);
	return ok;
}

bool gs_send_download_cmd(uint16_t len, uint8_t* buf, uint32_t fullLength, uint32_t targetAddr, bool more_coming) {
	uint8_t bodyHead[12];
	uint8_t seq_nbr = gs_seq_nbr++;
	bodyHead[ 0] = 0x81;
	bodyHead[ 1] = seq_nbr;
	bodyHead[ 2] = (fullLength) & 0xff;
	bodyHead[ 3] = (fullLength >> 8) & 0xff;
	bodyHead[ 4] = (fullLength >> 16) & 0xff;
	bodyHead[ 5] = (fullLength >> 24) & 0xff;
	bodyHead[ 6] = (len) & 0xff;
	bodyHead[ 7] = (len >> 8) & 0xff;
	bodyHead[ 8] = (targetAddr) & 0xff;
	bodyHead[ 9] = (targetAddr >> 8) & 0xff;
	bodyHead[10] = (targetAddr >> 16) & 0xff;
	bodyHead[11] = (targetAddr >> 24) & 0xff;
	bool ok = gs_send_hi_command(GS_DOWNLOAD_SERVICE, 0, 12, bodyHead, len, buf);
	return ok;
}

/** sort of arbitrary read by gs_flashprogram first */
bool gs_ping_module() {
	if (!gs_send_read_cmd(0x05001000, 4, NULL)) return false;
	return true;
}

void gs_send_read_mac_cmd() {
	//TODO
}

bool gs_poll(void) {
	bool ok = true;
#if (GS_MANUAL_SSEL)
	write_pin(GS_MANUAL_SSEL_PIN, false);
#endif
	uint8_t rcv[30];
	ok = ssp_write_read_sync8(GS_SSP_HW, 30, NULL, rcv, GS_LSB_FIRST);
	if (ok) gs_parse_incoming(rcv, 30);
#if (GS_MANUAL_SSEL)
	write_pin(GS_MANUAL_SSEL_PIN, true);
#endif
	delay(MAIN_CLOCK_MHZ * 1000);
	return ok;
}

bool gs_program_firmware(void) {
	if (!gs_ping_module()) return false;
	return true;
	//TODO ******
//	uint8_t buf[] = { 1,2,3,4,5,6,7,8,9,10 };
//	gs_send_download_cmd(10, buf, 20, GS_SHRAM_BASE + 0x10000, true);
}
