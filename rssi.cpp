#include "rssi.h"
#include "global.h"
#include "wifi_sniffer.h"

void* rssi_sense(void*) {
	sniff();
}
