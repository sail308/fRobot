#include "motor.h"
#include <math.h>
#include "robotctrl.h"
#include "obstacle.h"
#include "vff.h"
#include "global.h"

#define MAX_DIS 4000
#define MIN_DIS 1200
#define MAX_LIN 1600
#define MIN_LIN 1000
#define MAX_ANG 20

#define SMOOTH_LEN 5

extern int g_state;
extern vector<float> Step_Heading;
extern int xy_rw[2];
extern int g_xy_predict[2];
extern unsigned long g_losttime;
extern int g_ultrasonic[11];

ull index_previous = 0;

int g_port;
int g_open = 0;

int lost_spd = 0;
int lost_rtt = 0;

void* motor_read(void*) {
	while(g_open == 0) {}
	readrobot(g_port);
}

float avg(float data[]) {
	float sum = 0;
	for(int i=0; i<SMOOTH_LEN; i++) {
		sum += data[i];
	}
	return sum/SMOOTH_LEN;
}
void* motor_control(void*) {

	//int port = wcopen();
	g_port = openrobot("/dev/ttyUSB0", 19200);
	g_open = 1;
	printf("  p= %d\n", g_port);

	float orig_x = 0.0;
	float orig_y = 0.0;

	float dis_arr[SMOOTH_LEN];
	float ang_arr[SMOOTH_LEN];
	int idx = 0;

	while(true) {
//		cout << "g_state " << g_state << endl;
		if(g_state == 1 || g_state == 6) {
			float xx = xy_rw[0] - orig_x;
			float yy = xy_rw[1] - orig_y;
			float dis = sqrt(xx*xx+yy*yy);
			float ang = acos(xx / dis);
/*
			int spd = dis*MAX_LIN/MAX_DIS;
			if(yy < 0) spd = -spd;
			int rtt = (ang-PI/2)/PI*MAX_ANG;
*/			
			int ang_deg = ang * 180 / PI;
			ang_deg = ang_deg - 90;
			
			dis_arr[idx] = dis;
			ang_arr[idx] = ang_deg;
			idx = (idx+1)%SMOOTH_LEN;
			
			dis = avg(dis_arr);
			ang_deg = avg(ang_arr);
				
			int spd, rtt;
			if(dis>MIN_DIS && dis<MAX_DIS) 
				spd = (dis-MIN_DIS)*(MAX_LIN-MIN_LIN)/(MAX_DIS-MIN_DIS) + MIN_LIN;
			else
				spd = 0;
			if(ang_deg>=-10 && ang_deg<=10)
				rtt = 0;
			else
				rtt = ang_deg*6/5;
			if(abs(rtt)>90)
				rtt = abs(rtt)/rtt*90;
			
//			int speed[] = {spd, rtt};
//			COMMAND cmd = vff(xy_rw, g_ultrasonic, speed);
//			spd = cmd.v;
//			rtt = cmd.w;

			lost_spd = spd;
			lost_rtt = rtt;
			cout << "position\t" << dis << "\t" << ang_deg << "\t" << spd <<"\t" << rtt << endl;
			//wccontrol(spd, rtt, port);
			controlrobot(spd, rtt, g_port);

			/*
			if(xy_rw[1] < 300) {	// too near, wheelchair should reverse
				if(xy_rw[0]<-10) {    // on left
					wccontrol(-200, -400, port);
				} else if(xy_rw[0]<10){		// reverse
					wccontrol(-400, 110, port);
				} else {	// on right
					wccontrol(-200, 400, port);
				}
			}else{ 					// too far, wheelchar should go forward
				if(xy_rw[0]<-10) {    // on left
					wccontrol(200, 400, port);
				} else if(xy_rw[0]<10){		// reverse
					wccontrol(400, 110, port);
				} else {			// on right
					wccontrol(200, -400, port);
				}
			}*/
		}
		/*else if (g_state == 3) {
			long length = Step_Heading.size()-1;
			if(index_previous>=length) {
				wccontrol(0,0, port);
				cout << "wc stay stationary!" << endl;
				index_previous = length;
			} else {
				for (int i = index_previous+1; i<=length; i++) {
					if(Step_Heading[i] > 3.0) {
						wccontrol(200, 30, port);
						cout<< "wc left turn" << endl;
					}else if(Step_Heading[i]<-3.0) {
						wccontrol(200, -30, port);
						cout<< "wc right turn" << endl;
					}else{					
						wccontrol(200, 0, port);
						cout<< "wc straight forward" << endl;
					}
				}
				index_previous = length;
			}
		} else if (g_state == 2) {
			cout << "manual recovery required!" << endl;
		}*/
		else if(g_state == 2) {
			unsigned long curtime = gettimestamp();
			if(curtime - g_losttime < 5000) {
				cout << "position\t" << "xxxx" << "\t" << "yyyy" << "\t" << lost_spd <<"\t" << lost_rtt << endl;
//				int speed[] = {lost_spd, lost_rtt};
//				COMMAND cmd = vff(g_xy_predict, g_ultrasonic, speed);
//				lost_spd = cmd.v;
//				lost_rtt = cmd.w;
				controlrobot(lost_spd, lost_rtt, g_port);
			}else{
				g_state = 3;
			}
		}
		else if(g_state!=0){
//			controlrobot(0, 0, g_port);
//			cout << "manual recovery required!" << endl;
		}
		usleep(40000);				
	}
}
