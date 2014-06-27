#include<opencv/cv.h>
#include<opencv/highgui.h>
#include<opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<stdio.h>
#include<iostream>

#include "Particle.h"
#include "objectDetection.h"
#include "obstacle.h"

using namespace std;
using namespace cv;

extern int rect_hei;
extern int rect_wid;
extern int g_state;

extern int videoid;
extern Mat g_frame;

float thresh_dete = 0.2;

//void HSVcolorhist(Mat &src, MatND &hist_normal);
int ObjectDete(MatND ini_feature, Mat &ini_state){
	cout << "detection start."<< endl;
	//VideoCapture cap(videoid);
	Mat frame;
	int result_det = 0;
/*
	for(int i = 0; i<40; i++){
		Mat framedel;
		cap >> framedel;
		imshow("detection del", framedel);
		waitKey(13);
	}*/
	for(int frame_index = 0; frame_index <1000; frame_index++){
		cout << "image detection index: " << frame_index << endl;
		frame = g_frame.clone();
//		cap >> frame;
		Point p;
		vector<int> rect_size(2);
		
		unsigned long ms = gettimestamp();
		result_det = slidingwindow(frame, ini_feature, p, rect_size, rect_wid, rect_hei);
		cout << "Detection time: " << gettimestamp() - ms << endl;
		if(result_det)
		{
			ini_state.at<float>(0,0) = float(p.x);
			ini_state.at<float>(1,0) = float(p.y);	
			return 1;
		}
//		usleep(1000000);		
		imshow("Detection",frame);
		waitKey(8);
	}
	return 0;
}


int slidingwindow(Mat frame, MatND hist, Point &p, vector<int> &rect_size, int rectt_wid, int rectt_hei){               //p lower left point
	int cols = frame.cols;
	int rows = frame.rows;
	Mat framet = frame.clone();

	int cols2 = 0;
	int rows2 = 0;
	int step = 20;
	int scalar = 0;
	float confidence = 0.0;

	vector<Mat>  result(3);
	MatND feature;
	float minimum =2000;
	Point minimumloc,minimumloc_ori;
	for(int i=0; i<3; i++){
		cols2 = (i+1)*cols;
		rows2 = (i+1)*rows;
		resize(frame, framet, Size(), i+1,i+1,INTER_LINEAR);
		for(int j =0; j<rows2 - rectt_hei +1; j+=step){
			if(j+rectt_hei > rows2){
				cout <<"j" <<j<<endl;
				break;
			}
			for(int k=0; k<cols2 - rectt_wid + 1; k+=step){
				if(k + rectt_wid> cols2 ){
					cout <<"k break"<<endl;
					break;
				}
				Mat subimg = framet(Rect(k,j, rectt_wid, rectt_hei)).clone();
				HSVcolorhist(subimg, feature);
				float dis = 0;
				//result[i].at<float>(row_result,col_result) = compareHist( hist, feature, 3 );
				dis = compareHist( hist, feature, 3 );

				//if(result[i].at<float>(row_result,col_result) < minimum){
				if(dis < minimum){
					//minimum = result[i].at<float>(row_result,col_result);
					minimum = dis;
					minimumloc.x = k/(i+1);
					minimumloc.y = j/(i+1);
					scalar = i+1;
					minimumloc_ori.x = k;
					minimumloc_ori.y = j;
				}
			}
		}
		if(i >0)
			cout <<"image is resized. "<<endl;
	}
	//vectorfloat minval;
	vector<float> minval_v(3);
	vector<Point> minloc_v(3);
	double minval,maxval;
	Point minloc, maxloc;
	/// test minmaxloc
	float testmatrix[3][3] = {{9.001,8.9,7.11},{5.2,6.1,0.007},{2.1,0.0002,9}};
	Mat testmat = Mat(3,3,CV_32FC1,testmatrix);

	//minMaxLoc(result[0],&minval,&maxval, &minloc, &maxloc, Mat());
	//minMaxLoc(testmat,&minval,&maxval, &minloc, &maxloc, Mat());

	/*vector<Mat> test2(3);
	test2[0].create(3,4,CV_32FC1);
	test2[1].create(3,1,CV_32FC1);
	test2[2].create(1,4,CV_32FC1);
	cout <<"test mat: "<< test2[0].rows<<" "<< test2[0].cols<<endl;
	cout <<"test mat: "<< test2[1].rows<<" "<< test2[1].cols<<endl;
	cout <<"test mat: "<< test2[2].rows<<" "<<  test2[2].cols<<endl;*/

	rect_size[0] = rectt_wid/scalar;
	rect_size[1] = rectt_hei/scalar;
	p = minimumloc;
	p.y = p.y + rect_size[1];
	confidence =  10 * exp(-13*minimum*minimum);
	cout << "current confidence: " << confidence << endl;
	if(confidence > thresh_dete){
		return 1;
	}
	else
		return 0;
	cout <<"val: "<< minimum <<"   "<<"confidence: "<< confidence<<endl;
}
