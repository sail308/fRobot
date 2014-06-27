#include "db_utils.h"

#define CMD_LEN 1024
#define DURATION_AVERAGE_RSSI 0.5

static struct sqlite3* db = 0; //XXX: Thread safe ??? Previous tests failed without mutex

static const char * create_table_cmd  = \
  "CREATE TABLE IF NOT EXISTS rssi_table("
  "id INTEGER PRIMARY KEY,"
  "ts REAL,"
  "src TEXT,"
  "dst TEXT,"
  "rssi INTEGER);";

static const char * insert_cmd = "INSERT INTO rssi_table VALUES(%d, %ld.%06ld, '%s', '%s', %d);";
static const char * pre_query_cmd = \
  "SELECT COUNT(DISTINCT src) as count FROM rssi_table "
  "WHERE ts BETWEEN %.06f and %.06f;";
static const char * query_cmd = \
  "SELECT src, SUM(rssi)/count(*) FROM rssi_table "
  "WHERE ts BETWEEN %.06f and %.06f "
  "GROUP BY src;";

static const char * query_stat_cmd =                \
  "SELECT COUNT(*) FROM rssi_table "
  "WHERE ts BETWEEN %.06f and %.06f;";

static const char * delete_cmd = \
  "DELETE FROM rssi_table "
  "WHERE ts BETWEEN 0 and %.06f;";


/* Returns -1 on failure, 0 otherwise */
int db_init()
{
  printf("[ db_utils ]: %s, thread safe: %d\n", sqlite3_libversion(), sqlite3_threadsafe());
  int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE|SQLITE_OPEN_NOMUTEX, NULL);

  char *errmsg = 0;
  if ( rc )
    {
      fprintf(stderr, "[ db_utils ]: Cannot open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return -1;
    }

  rc = sqlite3_exec(db, create_table_cmd, 0, 0, &errmsg);
  if (rc != SQLITE_OK)
    {
      fprintf(stderr, "[ db_utils ]: Cannot create table: %s\n", errmsg);
      sqlite3_free(errmsg);
      sqlite3_close(db);
      return -1;
    }

  return 0;
}

/* Returns -1 on failure, 0 otherwise */
int db_commit(int pkt_idx,
              const struct timeval *tv,
              char* src_addr,
              char* dst_addr,
              int rssi)
{

  if (db == 0)
    db_init();

  char db_cmd[CMD_LEN];
  char *errmsg = 0;

  sprintf(db_cmd, insert_cmd, pkt_idx, tv->tv_sec, tv->tv_usec, src_addr, dst_addr, rssi);
  
  int rc = sqlite3_exec(db, db_cmd, 0, 0, &errmsg);

  if (rc != SQLITE_OK)
    {
      fprintf(stderr, "[ db_utils ]: %s\n Error insert RSSI value into database @ %ld.%06ld. %s\n", db_cmd, tv->tv_sec, tv->tv_usec, errmsg);
      sqlite3_free(errmsg);

      return -1;
    }
}

/* Return number of matches */
char** db_query_rssi(struct timeval *tv, int *num_matches)
{
  int rc, num_rows;
  char db_cmd[CMD_LEN];
  char *errmsg = 0;
  sqlite3_stmt* stmt;

  if (db == 0)
    return 0;

  double tm_now = tv->tv_sec + 1e-6 * tv->tv_usec;

  sprintf(db_cmd, pre_query_cmd, tm_now - DURATION_AVERAGE_RSSI, tm_now);

  rc = sqlite3_prepare_v2(db,db_cmd, -1, &stmt, 0);
  if (rc)
    {
      fprintf(stderr, "[ db_utils ]: %s\n Error query number of distinct src mac from DB, %s.\n", db_cmd, sqlite3_errmsg(db));
      return NULL;
    }

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW)
    {
      num_rows = sqlite3_column_int (stmt, 0);
      *num_matches = num_rows;
      sqlite3_finalize(stmt);
    }
  else
    {
      sqlite3_finalize(stmt);
      return 0;
    }

  char **results = (char **) malloc(num_rows * sizeof(char *));

  sprintf(db_cmd, query_cmd, tm_now - DURATION_AVERAGE_RSSI, tm_now);

  rc = sqlite3_prepare_v2(db,db_cmd, -1, &stmt, 0);

  if (rc)
    {
      fprintf(stderr, "[ db_utils ]: Error query data from DB.\n");
      return 0;
    }

  int cols = sqlite3_column_count(stmt);

  for (int i = 0; i < num_rows; i++)
    {
      rc = sqlite3_step(stmt);

      if ( rc == SQLITE_ROW )
        {
          results[i] = (char *)malloc(CMD_LEN);

          const char *eid = (const char*) sqlite3_column_text(stmt, 0);
          const char *rssi = (const char*) sqlite3_column_text(stmt, 1);
          
          sprintf(results[i], "%lld,%s,0,%s", (long long int)tv->tv_sec * 1000 + tv->tv_usec / 1000, eid, rssi);
        }

      else if ( rc == SQLITE_DONE )
        break;

      else
        {
          fprintf(stderr, "[ db_utils ]: Error query data from DB when stepping, %s.\n", sqlite3_errmsg(db));
          break;
        }
    }

  sqlite3_finalize(stmt);
  return results;
}


/* Return number of sniffed packets in the past minute */
void db_query_stat(struct timeval *tv, double *num_entries_per_sec)
{
  char db_cmd[CMD_LEN];
  sqlite3_stmt* stmt;
  int rc;

  double tm_now = tv->tv_sec + 1e-6 * tv->tv_usec;  

  sprintf(db_cmd, query_stat_cmd, tm_now - 3, tm_now);
  rc = sqlite3_prepare_v2(db,db_cmd, -1, &stmt, 0);

  if (rc)
    {
      fprintf(stderr, "[ db_utils ]: Error query data from DB.\n");
      return;
    }

  rc = sqlite3_step(stmt);

  if (rc == SQLITE_ROW)
      *num_entries_per_sec = (double)sqlite3_column_int (stmt, 0) / 3.0;
  else
      *num_entries_per_sec = 0.0;

  sqlite3_finalize(stmt);

  char* errmsg;
  sprintf(db_cmd, delete_cmd, tm_now - 3);
  rc = sqlite3_exec(db, db_cmd, 0, 0, &errmsg);

  if (rc != SQLITE_OK)
    {
      fprintf(stderr, "[ db_utils ]: Cannot delete data: %s\n", errmsg);
      sqlite3_free(errmsg);
    }

  return;
}
