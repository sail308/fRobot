#ifndef MOTION_H
#define MOTION_H

#include "global.h"
#include "comm.h"
using namespace std;


class Pedometer {

private:
	vector<ull> Step_Time;
	long Step_Count;
	vector<float> Step_Length;
	bool E_flag;

	vector<ull> tss1;
	vector<ull> tss2;
	vector<float> a_g1;
	vector<float> an_g1;
	vector<float> a_g2;
	vector<float> an_g2;

public:
	Pedometer();
	vector<float> fir_filter(vector<float> raw_data);
	void run();
};

void* motion_sense(void*);

extern int g_state;
#endif
