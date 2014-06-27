#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <random>
#include <numeric>
#include <math.h>
using namespace std;
using namespace cv;

//int visual_tracking();
int visual_tracking(MatND ini_feature, Mat ini_state);
int registration(MatND &ini_feature, Mat &ini_state);

#endif
