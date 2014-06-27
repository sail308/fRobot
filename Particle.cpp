/*
 * Particles.cpp

 *
 *  Created on: May 1, 2014
 *      Author: ying
 */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "Particle.h"

using namespace std;
using namespace cv;



Particle::Particle(Mat &sta)
{
	state = sta.clone();
	weight = 0;
}
Particle::Particle()
{
	state = (Mat_<float>(4,1) << 0,0,0,0);
	weight = 0;
}
Particle :: Particle(float x, float y, float xv, float yv, float w)
{
	state = (Mat_<float>(4,1) << x,y,xv,yv);
	weight = w;
}
void Particle::setState(Mat state_t){
	state = state_t.clone();
}
void Particle :: setWeight(float w)
{
	weight = w;
}
float Particle :: getWeight(){
	return weight;
}
Mat Particle::getState(){
	return state;
}

/*void Particle :: stateUpdate(Mat &speed){
	// to be continued...
}*/

//void HSVcolorhist(Mat &src, vector<float> &hist_feature){
void HSVcolorhist(Mat &src, MatND &hist_normal){
	Mat hsv;

	cvtColor(src, hsv, CV_BGR2HSV);

    //int hbins = 30, sbins = 32;
    int hbins = 10, sbins = 16;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180};
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };

    const float* ranges[] = { hranges, sranges };
    //MatND hist,hist_normal;
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};
    calcHist( &hsv, 1, channels, Mat(), // do not use mask
             hist, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
    normalize( hist, hist_normal, 0, 1, NORM_MINMAX, -1, Mat() );
    float maxVal=0;
    //minMaxLoc(hist, 0, &maxVal, 0, 0);

    //int scale = 10;
    /// reshape the histogram to 1-D array
    /*int index = 0;
    for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
        	//float binVal = hist.at<float>(h, s)/maxVal;
            float binVal = hist_normal.at<float>(h, s);
        	hist_feature[index] = binVal;
        	index++;
        }*/
}
void RGBcolorhist(Mat &src, MatND &hist_normal){
	Mat hsv = src.clone();

	//cvtColor(src, hsv, CV_BGR2HSV);

    //int hbins = 30, sbins = 32;
    int hbins = 10, sbins = 16, vbins = 16;
    int histSize[] = {hbins, sbins, vbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 256};
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    
    float vranges[] = { 0, 256 };

    const float* ranges[] = { hranges, sranges ,vranges };
    //MatND hist,hist_normal;
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1, 2};
    calcHist( &hsv, 1, channels, Mat(), // do not use mask
             hist, 3, histSize, ranges,
             true, // the histogram is uniform
             false );
    normalize( hist, hist_normal, 0, 1, NORM_MINMAX, -1, Mat() );
    float maxVal=0;
    //minMaxLoc(hist, 0, &maxVal, 0, 0);

    //int scale = 10;
    /// reshape the histogram to 1-D array
    /*int index = 0;
    for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
        	//float binVal = hist.at<float>(h, s)/maxVal;
            float binVal = hist_normal.at<float>(h, s);
        	hist_feature[index] = binVal;
        	index++;
        }*/
}

