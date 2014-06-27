#ifndef OBJECTDETECTION_H
#define OBJECTDETECTION_H

int ObjectDete(MatND ini_feature, Mat &ini_state);
int slidingwindow(Mat frame, MatND hist, Point &p, vector<int> &rect_size, int rectt_wid, int rectt_hei);

#endif
