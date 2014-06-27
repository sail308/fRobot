#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "obstacle.h"
#include "robotctrl.h"
using namespace std;


#define 	dis 		               180		 // 180mm, the dis between 2 recieve sonar sensor

int g_ultrasonic[11];

extern int xy_rw[2];
extern int g_state;

void cal_us_pos(int cur_L, int cur_R) {
	if(cur_L>3000 || cur_R>3000) {
		xy_rw[0] = 0;
		xy_rw[1] = 4000;
		return;
	}
    int   last_x, last_y;
    int   cur_x, cur_y, cur_theta, temp;
    int   d_line, dis_err, theta_err;
    int   v, w;
    float S, H, P, Ltheta, cosine;
    float L, R, D;

    L = cur_L * 1.0;
    R = cur_R * 1.0;
    D = dis * 1.0;

    P = (L + R + D) / 2;
    S = sqrt( P * (P-L) * (P-R) * (P-D) );
    H = 2 * S / D;
    cosine = (L*L + D*D - R*R)/(2.0*L*D);
    Ltheta = acos(cosine);
    
    cur_x = L * cosine - D/2.0;
    cur_y = H;
    d_line = sqrt(cur_x*cur_x + cur_y*cur_y);

    xy_rw[0] = cur_x;
    xy_rw[1] = cur_y;
}

void readultrosonic(int port) {
	char raw_buf[512];
	char b;
	int nread;
	int x, y, i, j, k, p, q;
	int num_len = 0, data_num = 0, buf_len = 0;
	char buf[128];
	int start_pos, end_pos, prev_pos;
	while(1) {
		if((nread=read(port, raw_buf, 512)) > 0) {
			for(i=0; i<nread; i++) {
				b = raw_buf[i];
				if(b=='\n') {
					prev_pos = 0;
					data_num = 0;
					for(j=0; j<=buf_len; j++) {
						if(buf[j] == ' ' || j == buf_len) {
							num_len = j-prev_pos;
							x=0;
							for(k=prev_pos, p=num_len-1; k<j; k++, p--) {
								y = buf[k]-'0';
								for(q=0; q<p; q++) y*=10;
								x+=y;
							}
							prev_pos = j+1;
//							printf("%d, ", x);
							g_ultrasonic[data_num] = x;
							data_num++;
						}
					}
					for(k=0; k<data_num; k++) {
						printf("%d ", g_ultrasonic[k]);
					}
					unsigned long ms = gettimestamp();
					
					printf("%lu\n", ms);
					
					if(data_num == 11 && g_state == 6) {
						cal_us_pos(g_ultrasonic[9], g_ultrasonic[10]);
					}
					buf_len = 0;
				} else {
					buf[buf_len] = b;
					buf_len++;
				}
			}
			memset(raw_buf, 0, sizeof(raw_buf));
		}
	}
}

void* obsread(void*) {
	int port = openrobot("/dev/ttyUSB1", 9600);
	readultrosonic(port);
}

unsigned long gettimestamp() {
	struct timeval  tm;
	unsigned long ms;
	gettimeofday(&tm, NULL);
	ms = tm.tv_sec * 1000 + tm.tv_usec / 1000;
	return ms;
}
