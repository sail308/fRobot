/*
 * particle.h
 *
 *  Created on: May 1, 2014
 *      Author: ying
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void HSVcolorhist(Mat &src, MatND &hist_feature);
void RGBcolorhist(Mat &src, MatND &hist_normal);

class Particle
{
public:
	Particle(Mat &sta);
	Particle();
	Particle(float x, float y, float xv, float yv, float w);

	void setState(Mat state_t);
	void setWeight(float w);
	float getWeight();
	Mat getState();
	void stateUpdate(Mat &speed);


private:
	Mat state;
	float weight;
};




#endif /* PARTICLE_H_ */
