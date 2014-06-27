#ifndef DB_UTILS_H
#define DB_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

char** db_query_rssi(struct timeval *tv, int *num_matches);

void db_query_stat(struct timeval *tv, double *num_entries_per_sec);

int db_commit(int pkt_idx,
              const struct timeval *tv,
              char* src_addr,
              char* dst_addr,
              int rssi);

#endif	//DB_UTILS_H
