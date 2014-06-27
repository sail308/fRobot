/*
 * ParticleFilter.cpp
 * 
 * This code includes registration and object tracking.
 * 
 * xy_rw stores the real world position of human feet.
 *  
 */

#include "ParticleFilter.h"
#include "Particle.h"
#include "obstacle.h"
#include <queue>
using namespace std;

#define MAX_QUEUE_SIZE 40

float vec_distance(vector<float>v1,vector<float>v2);
int weight_calc(Particle p,Mat frame,float &weight,MatND ini_fea);
void cumsum(vector<float> x, vector<float> &res);
void ini_particles(Mat ini_state, vector<Particle> &particles,float ini_var);
float checkrange(float r,float max);
void resample(vector<Particle> &particles);
void stateUpdate(vector<Particle> &par, float update_var,float update_speed_var, float update_rect_var);
void weighted_mean(vector<Particle> particles, Mat &mean_state);
void coordinateRW(float u, float v, int* xz);
void particle_mask(Mat state_mean, vector<Particle> &particles, float mask_wid, float mask_hei, float scal = 1.0);

int particle_num = 500;
int feature_dim = 960;
int cols = 640;
int rows = 480;
unsigned long g_losttime = 0;

queue<int*> loc_queue;
unsigned long last_loc_time;

extern Mat g_frame;

//OSU extrinsic
//float M_array[3][4] = {
//			{-0.0187555067604681,-0.00276196577971972,0.00748736371235511,0.671553260001550},
//			{-0.000136961662767235,-0.0197568772646839,-0.00214871425952167,0.740408586764904},
//			{7.98150091587004e-08,-7.17584076300446e-06,2.22607121238518e-05,0.000956579406744151}};

//SJTU extrinsic
float M_array[3][4] = {
			{-0.0223103533203266,     -0.0106000116287087,    0.00325229662790310,    -0.09225789249282},
    		{0.000346467377031329,     0.000650399745877259,  0.0233687031121399,     -0.975075539783975},
    		{5.66998805302952e-07,    -3.06540700225666e-05,  8.27212258423959e-06,   -0.000644317327630690}};
	
int lost_flag = 0;		 // 1: lost  0: tracked
int start_flag = 0;      // 1: start 0: prepare
float thresh_lost1 = 0.15;
int xy_rw[2];    // real world position
int g_xy_predict[2];

int rect_wid = 120;  //84;         //65;          // 100;   //146  65;
int rect_hei = 60;  //45;         //80;   //110  45;     // size of the rectangle
		
extern int g_state;

int registration(MatND &ini_feature, Mat &ini_state){
	
	float ini_state_array[6] = {465,437,2,2,float(rect_wid), float(rect_hei)}; //{350,383,2,-4,float(rect_wid), float(rect_hei)};   //{233,420,2,-4,float(rect_wid), float(rect_hei)};  //{350,383,0,-10};    // initial state: position and speed/lowerleft point
	
	//VideoCapture cap("/home/temp/Videos/ParticleFielter/video2.wmv");
	//VideoCapture cap(videoid);
	Mat framecap;
	for(int i=0; i< 200; i++){
//		cap >> framecap;
		framecap = g_frame.clone();
		rectangle(framecap, Point(ini_state_array[0],ini_state_array[1]), Point(ini_state_array[0]+rect_wid,ini_state_array[1]-rect_hei),Scalar(255,0,0),3,8,0);
		imshow("Registration",framecap);
		waitKey(30);
	}
	
	//cout <<"HERE" <<endl;
	Mat frame;
//	cap >> frame;
	frame = g_frame.clone();	

	//destroyWindow("registration");
	///video mode
	/*Mat frame;
	cap >> frame;*/
	
	/// initial feature calculation
	Mat ini_state_temp = Mat(6,1,CV_32FC1,ini_state_array);
	ini_state = ini_state_temp.clone();
	Rect rect1 = Rect(ini_state.at<float>(0),ini_state.at<float>(1)-rect_hei+1,rect_wid,rect_hei);
	//MatND ini_feature;
	Mat roit1 = frame(rect1);
	HSVcolorhist(roit1,ini_feature);
	//RGBcolorhist(roit1,ini_feature);

	
	xy_rw[0] = 100;
    xy_rw[1] = 500;
    /// vision tracking begins, registering ends here
    start_flag = 1;
	g_state = 1;
	return 1;
}

int visual_tracking(MatND ini_feature, Mat ini_state) { 
	
	float x = 0;
	float y = 0;
	
	float ini_var = 10;
	float update_var = 15;
	float update_speed_var = 5.0;
	float update_rect_var = 3; //3
	
	int counter = 0;
	g_state = 1;        // change robot state
	
	Mat M = Mat(3,4,CV_32FC1, M_array);
	
	Particle p_temp1 = Particle(ini_state);
	vector <Particle> particles(particle_num,p_temp1);

	ini_particles(ini_state, particles, ini_var);
	Mat weighted_state = ini_state.clone();
	    
	//VideoCapture cap(videoid);
	Mat frame;
	     // update weights
	for(int frame_index = 0;frame_index <23000; frame_index++){
//		cap >> frame;
		frame = g_frame.clone();
		//cout <<"frame tracking: " << frame_index << endl;
		if(frame_index < 30){
			putText(frame, "Ready", Point(400,100), CV_FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0,255,0),2,8,false);
		}
		float sum = 0.0;
		float max_weight = 0.0;
		for (int i=0; i<particle_num; i++){
			Mat state_temp = particles[i].getState();
			float weight;
			weight_calc(particles[i],frame,weight,ini_feature);  // weight calculation for the particles
			particles[i].setWeight(weight);
			//cout << "weight " << particles[i].getWeight() << "position  "<<state_temp.at<float>(0,0) <<endl;
		}
		particle_mask(weighted_state, particles, 200, 150, 1.5);    
		
		///normalization
		for(int i=0; i<particle_num; i++){
			sum = sum + particles[i].getWeight();
			if(particles[i].getWeight() > max_weight){
				max_weight = particles[i].getWeight();
			}
		}
		if (max_weight < thresh_lost1){
			//cout <<"chane state " << endl;
			lost_flag = 1; 
			counter ++;                       /// lost check
			if(g_state == 1 && counter == 30)	
			{
				g_losttime = gettimestamp();
				g_state = 2;
				counter =0;
				cout << "change state " << g_state << endl;
				//break;
				return 0;
			}
		}
		cout << "max_weight "<< max_weight <<endl;
		for(int i =0; i<particle_num;i++){
			float ww = particles[i].getWeight();
			particles[i].setWeight(ww/sum);
			//cout<<"weight after normalization "<<particles[i].getWeight()<<endl;
		}
		weighted_mean( particles, weighted_state);
		x = weighted_state.at<float>(0,0);
		y = weighted_state.at<float>(1,0);
		rectangle(frame, Point(x,y), Point(x+weighted_state.at<float>(4,0),y-weighted_state.at<float>(5,0)),Scalar(255,0,255),2,8,0);
		coordinateRW(x,y,xy_rw);
		unsigned long ms = gettimestamp();
		if(ms-last_loc_time > 100) {
			int* xy_tmp = new int[2];
			xy_tmp[0] = xy_rw[0];
			xy_tmp[1] = xy_rw[1];
			loc_queue.push(xy_tmp);
			last_loc_time = ms;
			if(loc_queue.size() > MAX_QUEUE_SIZE)
				loc_queue.pop();
		}
		int xs=0, ys=0;
		for(int i = 0; i<loc_queue.size(); i++) {
			int* tmp = loc_queue.front();
			loc_queue.pop();
			xs+=tmp[0];
			ys+=tmp[1];
			loc_queue.push(tmp);
		}
		g_xy_predict[0] = xs/loc_queue.size();
		g_xy_predict[1] = ys/loc_queue.size();
		cout << g_xy_predict[0] << "," << g_xy_predict[1] << endl;
		cout <<"coordinate: " << x << " " << y << " " << xy_rw[0] <<" " << xy_rw[1] << endl;
		//start_flag = 1;
		//if(g_)
		//g_state = 1;                
		/// lost check
		/*if(x + weighted_state.at<float>(4,0) >= rows) {
			lost_flag = 1;
			if(g_state == 1)	
				g_state = 2;
		}*/
		
		
		///resample
		resample(particles);
		
		///draw results
		for(int i=0; i<particle_num; i++){
			Mat state1 = particles[i].getState();
			circle( frame, Point(state1.at<float>(0,0),state1.at<float>(1,0)), 2, Scalar(255,255,0), CV_FILLED, 8,0 );
		}
		
		/// update state
		stateUpdate(particles,update_var,update_speed_var,update_rect_var);
		//cout<<"weighted size "<< weighted_state.at<float>(4,0) <<" "<< weighted_state.at<float>(5,0) <<endl;
		imshow("Tracking",frame);
		waitKey(15);
		//cout<<"end of first round!" << endl;
		//if(waitKey(10)>0) break;
		if(g_state == 0)
			break;
	}
	return 1;
}

float vec_distance(vector<float>v1,vector<float>v2){
	int dim = v1.size();
	float di = 0;
	for (int i =0; i<dim; i++){
		di = di + (v1[i]-v2[i])*(v1[i]-v2[i]);
	}
	return sqrt(di);
}
int weight_calc(Particle p,Mat frame,float &weight,MatND ini_fea){
	Mat state = p.getState().clone();
	float sca = 0.6;
	if(state.at<float>(4,0) < 0 || state.at<float>(5,0) < 0)
	{
		weight = sca * p.getWeight();
		return 0;
	}
	if(state.at<float>(4,0) > cols || state.at<float>(5,0) > rows)
	{
		weight = sca * p.getWeight();
		return 0;
	}
	if((state.at<float>(0,0)+state.at<float>(4,0)-1)<cols && (state.at<float>(1,0)-state.at<float>(5,0)+1)>=0){
	//if((state.at<float>(0,0)+rect_wid-1)<cols && (state.at<float>(1,0)-rect_hei+1)>=0){
		Mat subimg = frame(Rect(state.at<float>(0,0),state.at<float>(1,0)-state.at<float>(5,0)+1,state.at<float>(4,0), state.at<float>(5,0)));
		//Mat subimg = frame(Rect(state.at<float>(0,0),state.at<float>(1,0)-rect_hei+1,rect_wid, rect_hei));
		MatND par_feature;
		HSVcolorhist(subimg,par_feature);
		//RGBcolorhist(subimg, par_feature);
			//weight = vec_distance(par_feature,ini_fea);
//		cout<< "1: " << ini_fea.type() << "; 2: " << par_feature.type() << endl;
		weight = compareHist( ini_fea, par_feature, 3 );
		//weight = 1.0/(1.0*weight);
		weight =  10 * exp(-13*weight * weight);
		return 1;
	}
	else{
		weight = sca*p.getWeight();                     // to be altered.
		return 0;
	}
}

void cumsum(vector<float> x, vector<float> &res)
{
	partial_sum(x.begin(), x.end(), res.begin());
}

void resample(vector<Particle> &particles)
{
	vector<Particle> particles2 = particles;
	vector<float> weights(particles2.size());
	vector<float> cmd_P_w(particles2.size());
	float u = 0;
	for(int i=0; i<particle_num; i++){
		weights[i] = particles[i].getWeight();
	}

	cumsum(weights, cmd_P_w);
	default_random_engine generator;
	uniform_real_distribution<float> distribution(0.0, 1.0/float(particle_num));
	float u1 = distribution(generator);

	int i = 0;
	for (int j = 0; j != particle_num; j++)
	{
		u = u1 + float(float(j)*(1.0/particle_num));
		while (u > cmd_P_w[i])
		{
			i += 1;
		}
		particles[j].setWeight(particles2[i].getWeight());
		particles[j].setState(particles2[i].getState());
		//*particles[j] = *particles2[i];
		//TODO:
	}
	for(int i=0; i<particle_num; i++){
		weights[i] = particles[i].getWeight();
	}
	float sum_weight = 0;
	sum_weight = accumulate(weights.begin(), weights.end(), 0.);
	for(int i=0; i<particles.size(); i++){
		weights[i] = weights[i]/sum_weight;
		particles[i].setWeight(weights[i]);
	}
}
void ini_particles(Mat ini_state, vector<Particle> &particles,float ini_var){
	default_random_engine generator;
	normal_distribution<float> distribution(0.0, ini_var);
	Mat statetemp = ini_state.clone();
	for (int i=0; i<particle_num; i++){
		statetemp.at<float>(0,0) = checkrange(float(ini_state.at<float>(0,0)+distribution(generator)),cols);
		statetemp.at<float>(1,0) = checkrange(float(ini_state.at<float>(1,0)+distribution(generator)),rows);
		particles[i].setState(statetemp);
		particles[i].setWeight(1.0/particle_num);
			//particles[i] = Particle(checkrange(float(ini_state_array[0]+distribution(generator)),cols),checkrange(float(ini_state_array[1]+distribution(generator)),rows),ini_state_array[2],ini_state_array[3],1.0/particle_num);
			//particles[i].getState().at<float>(0,0) = checkrange(float(ini_state_array[0]+distribution(generator)),cols);
			//particles[i].getState().at<float>(1,0) = checkrange(float(ini_state_array[1]+distribution(generator)),rows);
			
		}
}
float checkrange(float r,float max){
	if(r < 0)
		return 0;
	else
		if(r > max - 1)
			return max - 1;
		else
			return floor(r);
}
void stateUpdate(vector<Particle> &par, float update_var,float update_speed_var, float update_rect_var){
	default_random_engine generator, generator2, generator3;
	normal_distribution<float> distribution(0.0, update_var);
	normal_distribution<float> distribution2(0.0, update_speed_var);
	normal_distribution<float> distribution3(0.0, update_rect_var);
	for(int i=0; i<par.size(); i++){
		Mat state1 = par[i].getState().clone();
		float uu1 = distribution(generator);
		float uu2 = distribution(generator);
		float ss1 = distribution2(generator2);
		float ss2 = distribution2(generator2);
		float rr1 = distribution3(generator3);
		float rr2 = distribution3(generator3);
		state1.at<float>(0,0) = checkrange(float(state1.at<float>(0,0)+ state1.at<float>(2,0)+uu1),float(cols));
		//cout <<"position0 "<< state1.rows<<endl;
		state1.at<float>(1,0) = checkrange(float(state1.at<float>(1,0)+ state1.at<float>(3,0)+uu2),float(rows));
		state1.at<float>(2,0) = state1.at<float>(2,0) + ss1;
		state1.at<float>(3,0) = state1.at<float>(3,0) + ss1;
		state1.at<float>(4,0) = checkrange(float(state1.at<float>(4,0)+ rr1),float(cols));
			//cout <<"position0 "<< state1.rows<<endl;
		state1.at<float>(5,0) = checkrange(float(state1.at<float>(5,0)+ rr2),float(rows));
		if(state1.at<float>(4,0) < 20){
			state1.at<float>(4,0) = 20;
		}
		if(state1.at<float>(5,0) < 15){
			state1.at<float>(5,0) = 15;
			//state1.at<float>(5,0) = 7;
		}
		par[i].setState(state1);
		//cout<<"rr1 "<< rr1 <<"   rr2 "<< rr2<<endl;
	}
}
void weighted_mean(vector<Particle> particles, Mat &mean_state){
	Mat statem =Mat::zeros(6,1,CV_32FC1);
	
	for(int i=0; i< particle_num; i++){
		//Mat temp = particles[i].getState().clone();
		//mx = mx + particles[i].getState().at<float>(0,0)*particles[i].getWeight();
		//my = my + particles[i].getState().at<float>(1,0)*particles[i].getWeight();
		statem = statem + particles[i].getState()*particles[i].getWeight();
		//cout <<"particle size  "<<particles[i].getState().at<float>(4,0) <<" "<<"at (5,0) " << particles[i].getState().at<float>(5,0) << endl;
	}
	//x = floor(mx);
	//y = floor(my);
	statem.at<float>(0,0) = floor(statem.at<float>(0,0));
	statem.at<float>(1,0) = floor(statem.at<float>(1,0));
	mean_state = statem.clone();
}
void coordinateRW(float u, float v, int* xz){          // output real world coordinates (x,z)
/*	float U[3][4],U_t[3][4];
	for(int i=0;i<4;i++){
	   U[0][i] = u*M_array[2][i]-M_array[0][i];
	   U[1][i] = v*M_array[2][i]-M_array[1][i];
	}
	U[2][0] = 0;
	U[2][1] = 1;
	U[2][2] = 0;
	U[2][3] = 0;
	xz[1] = (U[1][0]*U[0][3]-U[0][0]*U[1][3])/(U[0][0]*U[1][2]-U[1][0]*U[0][2]);
	xz[0] = -1*(U[0][2]*xz[1]+U[0][3])/U[0][0];
*/
	float a = u*M_array[2][0] - M_array[0][0];
	float b = u*M_array[2][1] - M_array[0][1];
	float c = u*M_array[2][3] - M_array[0][3];
	float d = v*M_array[2][0] - M_array[1][0];
	float e = v*M_array[2][1] - M_array[1][1];
	float f = v*M_array[2][3] - M_array[1][3];
	xz[0] = (b*f-c*e)/(a*e-b*d)*10;
	xz[1] = (a*f-c*d)/(b*d-a*e)*10;
}
void particle_mask(Mat state_mean, vector<Particle> &particles, float mask_wid, float mask_hei, float scal)  //  scal: increase the weight
{
	float edgel = checkrange(state_mean.at<float>(0,0) - mask_wid/2, cols);
	float edger = checkrange(state_mean.at<float>(0,0) + mask_wid/2, cols);
	float edgelow = checkrange(state_mean.at<float>(1,0) + mask_hei/2, rows);
	float edgeu = checkrange(state_mean.at<float>(1,0) - mask_hei/2, rows);
	float weightt = 0;
	for(int i=0; i< particle_num; i++){
		Mat statet = particles[i].getState().clone();
		weightt = particles[i].getWeight();
		if(statet.at<float>(0,0) < edger && statet.at<float>(0,0) > edgel  && statet.at<float>(1,0) < edgelow && statet.at<float>(1,0) > edgeu)
		particles[i].setWeight(weightt * scal);
	}
}
