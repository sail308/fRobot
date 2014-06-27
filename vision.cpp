#include "vision.h"
#include "global.h"
#include "ParticleFilter.h"
#include "objectDetection.h"

extern int g_state;

int videoid = 0;
Mat g_frame;
int video_ready = 0;

void* vision_sense(void*) {
	MatND ini_feature;
	Mat ini_state;
	VideoCapture cap(videoid);
	while(video_ready == 0) {}
	while(true) {
		if(g_state == 0) {
			registration(ini_feature, ini_state);
			visual_tracking(ini_feature, ini_state);
			cout << "init type: " << ini_feature.type() << endl;
		} else if(g_state == 2 || g_state == 3) {
			// visual detection
			//g_state = 0; 
			int flag = ObjectDete(ini_feature, ini_state);
			//cout << "detecting" << endl;
			if(flag)
				visual_tracking(ini_feature, ini_state);
			else
				cout << "Failed to detect the label in given time."<<endl;
		}
//		usleep(1000000);              //
	}
}

void* video_capture(void*) {
	VideoCapture cap(videoid);
	while(1) {
		cap.read(g_frame);
		waitKey(30);
		video_ready = 1;
	}
}
