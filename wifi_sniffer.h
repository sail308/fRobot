#ifndef WIFI_SNIFFER_H
#define WIFI_SNIFFER_H



#include <pcap.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

extern "C"
{	
	#include "radiotap-parser.h"
};
#include "ieee80211.h"
#include "db_utils.h"
#include "comm.h"

// To stop the wifi thread

void sniff();

extern int g_state;

extern "C"
{
	extern int ieee80211_radiotap_iterator_init(ieee80211_radiotap_iterator*, ieee80211_radiotap_header*, int);
	extern int ieee80211_radiotap_iterator_next(ieee80211_radiotap_iterator*);
};

#endif
