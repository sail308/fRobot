#include <pthread.h>
#include "rssi.h"
#include "motion.h"
#include "vision.h"
#include "obstacle.h"
#include "motor.h"
using namespace std;

pthread_t thread_rssi = 0;
pthread_t thread_motion = 0;
pthread_t thread_video = 0;
pthread_t thread_vision = 0;
pthread_t thread_motor = 0;
pthread_t thread_position = 0;
pthread_t thread_obstacle = 0;

//g_state:
// 	registration	0
//	vision			1
//	visionlost		2
// 	motion			3
int g_state = 0;

int main() {
	void* motion_result;
	void* rssi_result;

//	pthread_create(&thread_rssi, NULL, rssi_sense, NULL);
//	pthread_create(&thread_motion, NULL, motion_sense, NULL);
	pthread_create(&thread_video, NULL, video_capture, NULL);
	pthread_create(&thread_vision, NULL, vision_sense, NULL);
	pthread_create(&thread_motor, NULL, motor_control, NULL);
//	pthread_create(&thread_position, NULL, motor_read, NULL);
//	pthread_create(&thread_obstacle, NULL, obsread, NULL);
	while(1) {usleep(1000000);}
	return 0;
}
