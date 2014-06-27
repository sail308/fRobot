#include "wifi_sniffer.h"

#define CMD_LEN 1024
#define DURATION_AVERAGE_RSSI 500000L

#define MAX_WHITE_LIST_LEN 20

const char* url = "tcp://192.168.1.100:5555";
const char* ctrl_url = "tcp://192.168.1.100:5556";

int zone_id = 1;
int dev_id = 0;
char *ip_addr = "";


// white list of mac addr to send
// littlestrong
int white_list_len = 0;
char white_list[128][MAX_WHITE_LIST_LEN];
const char *white_list_file = "wht_list.txt";


void loadWhtList() {
	FILE *fp = fopen(white_list_file, "r");
	if (fp == NULL) {
		printf("Cannot read white list file %s\n", white_list_file);
		exit(1);
	}
	
	white_list_len = 0;
	char tmp_str[128];
	while (fscanf(fp, "%s\n", tmp_str) != EOF) {
		if(tmp_str[0] == '#')
			continue;
		
		strcpy(white_list[white_list_len], tmp_str);
		printf("Load %s to white list\n", white_list[white_list_len]);
		white_list_len += 1;
		if (white_list_len>=MAX_WHITE_LIST_LEN) {
			printf("Meet max white list length limit %d\n", MAX_WHITE_LIST_LEN);
			break;
		}
	}
	fclose(fp);
	fp = NULL;
}

int inWhiteList(const char *eid) {
	int i = 0;
	for (i=0; i<white_list_len; i++)
		if (strcmp(eid, white_list[i]) == 0)
			return 1;
	return 0;
}


int init_pcap() {
  	static char errbuf[PCAP_ERRBUF_SIZE];
  	pcap_if_t *alldevs;
   
  	int ip_len = 0, rc = 0;
 	char *device="wlan0";
   
	char *ip_id = (char*) malloc(3);
  	int ip_id_factor = 200;
   
  	int status = pcap_findalldevs(&alldevs, errbuf);
  	if(status != 0) {
    	printf("%s\n", errbuf);
    	return -1;
  	}
   
  	for(pcap_if_t *d=alldevs; d!=NULL; d=d->next) {
//      	printf("flag1\n");
      	if(!strcmp(d->name, device) )
      	{            
        	for(pcap_addr_t *a=d->addresses; a!=NULL; a=a->next) 
          	{
//				printf("flag2\n");
            	if(a->addr->sa_family == AF_INET)
              	{
                	ip_addr = inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr);
                	ip_len = strlen(ip_addr);
                	if (!ip_len)
                  	{
                    	rc = -2;
                    	break;
                  	}
	          		strncpy(ip_id, ip_addr+ip_len-3, 3);
  
//					printf("flag3:%s\n", ip_addr);
              		dev_id = atoi(ip_id) - ip_id_factor;         
              	}
          	}       
      	}
  	}
  	pcap_freealldevs(alldevs);

	loadWhtList();

  	return rc;
}


void send_rssi()
{
  	int num_matches = 0;
  	char ** results;
  	static struct timeval ts0;
  	static int initialized = 0;
  	struct timeval ts;
  	suseconds_t elapsed_usec;

  	if (!initialized)
    {
  	    initialized = 1;
  	    gettimeofday(&ts0, NULL);
    }

  	gettimeofday(&ts, NULL);
  	elapsed_usec = (ts.tv_sec - ts0.tv_sec) * 1000000 + (ts.tv_usec - ts0.tv_usec); 

  	if (elapsed_usec < DURATION_AVERAGE_RSSI)
  	  return;

  	memcpy(&ts0, &ts, sizeof(struct timeval));
  	results = db_query_rssi(&ts, &num_matches);

  	if (num_matches == 0)
   		return;
  
  	for (int i = 0; i < num_matches; ++i)
    {
      	char *msg = (char *)malloc(strlen(results[i]) + 5);
      	sprintf(msg, "%d,%s\n", dev_id, results[i]);
		string r(results[i]);
		int lastcomma = r.find_last_of(',');
		int rssi = -128;
		if(lastcomma != -1) {
			rssi = atoi(r.substr(lastcomma+1).c_str());
			if(rssi > -90 && g_state == 2)		// Threshold to be modified
				g_state = 3;
			else if(rssi < -90 && g_state != 1) {
				g_state = 0;
				cout << "Initialization required!\n";
			}
		}
//      	comm_data(url, msg, strlen(msg));
//        write(wifi_sockfd, msg, strlen(msg));
		cout << results[i] << endl;
      	free(msg);
      	free(results[i]);
    }
  	free(results);

//  	double num_entries_per_sec = 0.0;
//  	char stat_msg[CMD_LEN];

//  	db_query_stat(&ts, &num_entries_per_sec);
//  	sprintf(stat_msg, "%lld,%d,%d,%s,%.02f", (long long int)ts.tv_sec * 1000 + ts.tv_usec / 1000, dev_id, zone_id, ip_addr, num_entries_per_sec);  
//  	comm_ctrl(ctrl_url, stat_msg, strlen(stat_msg));
//    write(wifi_sockfd, msg, strlen(msg));
  
//  system("clear");
// 	printf("Client No.: %d @ %ld.%06ld send #raw pkts per sec: %.02f\n", dev_id, ts.tv_sec, ts.tv_usec, num_entries_per_sec);
};


void print_mac(char *mac_str, uint8_t* addr)
{
  sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", 
	addr[0],
	addr[1],
	addr[2],
	addr[3],
	addr[4],
	addr[5]);
  mac_str[IEEE80211_ADDR_LEN*2+5] = 0;
};

static void process_packet(u_char *user, const struct pcap_pkthdr *h,
                           const u_char *bytes) 
{

  	static u32 pkt_idx;

  	u_char* buf = (u_char *) malloc(h->caplen);
  	memcpy(buf, bytes, h->caplen);

  	struct ieee80211_radiotap_iterator* iterator = (struct ieee80211_radiotap_iterator*) malloc(sizeof(struct ieee80211_radiotap_iterator));
  	struct ieee80211_radiotap_header* radiotap_header = (struct ieee80211_radiotap_header *) buf;
  	struct ieee80211_frame* ieee80211_header = (struct ieee80211_frame *) (buf + radiotap_header->it_len);

  	int rc = 0, i, rssi = -255;

  	rc = ieee80211_radiotap_iterator_init(iterator, radiotap_header, h->caplen);
  	if (rc < 0)
    {
      	printf("Failed to initialize radiotap header!\n");

      	free(iterator);
      	free(buf);

      	return;
    }

  	u_char* present_flags = (u_char *)(&(iterator->bitmap_shifter));
  	while ((rc = ieee80211_radiotap_iterator_next(iterator)) >= 0)
    {
  	    if ( iterator->this_arg_index == IEEE80211_RADIOTAP_DBM_ANTSIGNAL )
        {
  	        rssi = *(s8 *)(iterator->this_arg);
          //          printf(" RSSI: %d\n", rssi);
        }
    }

  	if (rssi > -128 && rssi < 0)
    {
  	    char src_addr[IEEE80211_ADDR_LEN*2 + 6];
  	    char dst_addr[IEEE80211_ADDR_LEN*2 + 6];

  	    /* Despite of the From DS / To DS flags, 
  	       addr1 / addr2 correspond to receiver / transmitter mac address. 
  	       See IEEE 802.11 Reference: http://www.willhackforsushi.com/papers/80211_Pocket_Reference_Guide.pdf */

  	    print_mac(dst_addr, ieee80211_header->i_addr1);
  	    print_mac(src_addr, ieee80211_header->i_addr2);

      	if (inWhiteList(src_addr)) {
      		db_commit(pkt_idx++, &(h->ts), src_addr, dst_addr, rssi);
//			cout << src_addr << ": " << rssi << endl;
		}
    }

  	send_rssi();
  
  	free(iterator);
  	free(buf);

  	return;
};



void sniff()
{
	printf("this is wifi_sniffer\n");
  	int rc;
  	char errbuf[PCAP_ERRBUF_SIZE], *errmsg;


  	/* -------------------- Initialize Pcap --------------------*/
  	pcap_t *handle; 
  	char *dev = "mon0";
  	while ( init_pcap() != 0 );

  	struct pcap_pkthdr header;
  	const u_char *packet;
  	struct ether_header *ether;

  	handle = pcap_create(dev, errbuf);
  	if(handle == NULL) {
    	printf("pcap_create failed: %s\n", errbuf);
    	exit(EXIT_FAILURE);
  	}

  	pcap_set_promisc(handle, 1);  /* Turn promiscuous mode on to sniff all packets */
  	pcap_set_timeout(handle, 512); /* 512 milliseconds */

  	int status = pcap_activate(handle);
  	if(status != 0) {
    	pcap_perror(handle, "Error activating Pcap: ");
  	}

  	int dlt = pcap_datalink(handle);
  	fprintf(stderr, "link-type: %s\n", pcap_datalink_val_to_name(dlt));

  	while (true)
    {
      	const u_char* packet_data;
      	packet_data = pcap_next(handle, &header);
      	if (packet_data != NULL)
        process_packet(NULL, &header, packet_data);
    }


  	pcap_close(handle);
}
