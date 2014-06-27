#include "motion.h"
#include <sys/types.h>  
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>

vector<float> Step_Heading;

float FILTER[] = {-0.0029,-0.0007, 0.0055, 0.0166, 0.0327,
		           0.0526, 0.0740, 0.0942, 0.1098, 0.1184,
		           0.1184, 0.1098, 0.0942, 0.0740, 0.0526,
		           0.0327, 0.0166, 0.0055,-0.0007,-0.0029};
Pedometer::Pedometer(){}

vector<float> Pedometer::fir_filter(vector<float> raw_data) {
	vector<float> a_filtered;
	for(int i = 20; i < raw_data.size(); i++) {
		float sum = 0.0;
		for(int j = 0; j < 20; j++) {
			sum += FILTER[j]*raw_data[i-j];		
		}
		a_filtered.push_back(sum);
	}
	return a_filtered;
}

void Pedometer::run() {
	int sockfd, client_fd; 
    struct sockaddr_in remote_addr;
	sockfd = init_server_socket();
	cout << "socket created" << endl;	

	int N = 5;
    while (true) {
		int count = 0;
		int i;
		while(count<N) {
			// receiving
        	socklen_t sin_size = sizeof(struct sockaddr_in);
        	if ((client_fd = accept(sockfd, (struct sockaddr*) &remote_addr, &sin_size)) == -1) {  
        	    cout << "accept error!" << endl;  
        	    continue;  
        	}  

//        	printf("Received a connection from %s\n", (char*) inet_ntoa(remote_addr.sin_addr));  
      
        	int rval;  
        	char raw_data[1024];
			memset(raw_data, 0, 1024);
        	if ((rval = read(client_fd, raw_data, 1024)) < 0) {  
        		cout << "reading stream error!" << endl;  
        		continue;  
        	}  
        	printf("%s\n", raw_data);
        	close(client_fd);

			//tokenize
			vector<string> contents;
	  		char * pch;
  			pch = strtok (raw_data," ");
			contents.push_back(pch);
  			while (true)
  			{
//    			printf ("%s, %d\n",pch, (int)contents.size());
    			pch = strtok (NULL, " ");
				if(pch == NULL)
					break;
				contents.push_back(pch);
  			}

			// receive new raw data
			ull ts;
			float acc_g;
			float ang_g;
			i=5;

			while (i < contents.size()) {
				stringstream strval;
				strval << contents[i];
				strval >> ts;
					
				acc_g = atof(contents[i].c_str());
				i += 3;
					
				ang_g = atof(contents[i].c_str());
				i += 4;

				tss2.push_back(ts);
				a_g2.push_back(acc_g);
				an_g2.push_back(ang_g);
//				cout << "### " << count << endl;
			}
			count++;
		}	
	
		float sum = 0;
		float avg_g = 0;
		int j = 1;
		// Moving combining the raw data
		if(a_g1.size()<1)  {                     // 1st time listening
			for(i=0; i<tss2.size(); i++)	tss1.push_back(tss2[i]);
			for(i=0; i<a_g2.size(); i++)	a_g1.push_back(a_g2[i]);
			for(i=0; i<an_g2.size(); i++)	an_g1.push_back(an_g2[i]);
					
			tss2.clear();
			a_g2.clear();
			an_g2.clear();

			// taking 1/30 of total data for calculating static G	
			for( j=1; j<a_g1.size()/30; j=j++) {
				sum = sum+a_g1[j];
			}
			avg_g = sum/(j-1);
			E_flag = false;
		} else {		 // combine two data segments and count steps
			
			vector<ull> tss;
			vector<float> a_g;
			vector<float> an_g;

			for(i=0; i<tss1.size(); i++)	tss.push_back(tss1[i]);
			for(i=0; i<a_g1.size(); i++)	a_g.push_back(a_g1[i]);
			for(i=0; i<an_g1.size(); i++)	an_g.push_back(an_g1[i]);
			
			// the lists to be processed for step detection
			for(i=0; i<tss2.size(); i++)	tss.push_back(tss2[i]);
			for(i=0; i<a_g2.size(); i++)	a_g.push_back(a_g2[i]);
			for(i=0; i<an_g2.size(); i++)	an_g.push_back(an_g2[i]);

			// now filter the raw data in a_g and an_g
			vector<float> a_gs =  fir_filter(a_g);
			

			// calculating g_max g_min
			float g_max = a_gs[0];
			float g_min = a_gs[0];
			for( i=1; i<a_gs.size()-1; i=i++) {
				if(a_gs[i]>g_max)
					g_max = a_gs[i];
				if(a_gs[i]<g_min)
					g_min = a_gs[i];
			}

			// for storing step related value
			vector<int> p_index;
			vector<float> p_a_gs;
			vector<long> p_ts;

			vector<int> b_index;
			vector<float> b_a_gs;
			vector<float> step_esti;
			vector<float> step_angle_esti;

			// peak detection threshold
			float pek_thre = 10.50f;
			float btm_thre = (float)(avg_g-0.20);
					
			for( i=1; i<a_gs.size()-2; i=i++) {
				if ( a_gs[i]>a_gs[i-1] && a_gs[i]>a_gs[i+1] ) // detect peaks
				{
					if(a_gs[i] > pek_thre){
						if(Step_Count>0) {       // this is not the first time detection
							if(i>a_g1.size()-1) {
								p_index.push_back(i);
								p_a_gs.push_back(a_gs[i]);
								p_ts.push_back(tss[i]);
							}
						} else {
							p_index.push_back(i);
							p_a_gs.push_back(a_gs[i]);
							p_ts.push_back(tss[i]);
						}			
					}	
				}
				if ( a_gs[i]<a_gs[i-1] && a_gs[i]<a_gs[i+1] )  // detect bottoms
				{
					if(a_gs[i] < btm_thre) {
						b_index.push_back(i);
						b_a_gs.push_back(a_gs[i]);
					}	
				}			
			}
			E_flag = false;	

			// if there is step event
			if(p_index.size()>0) {
				cout << "$$$ step" << p_index.size() << p_a_gs.size() << p_ts.size()<<step_esti.size()<< step_angle_esti.size()<< ";  " << b_index.size() << "," << b_a_gs.size()  << endl;
				// estimate the step lengths
				for(i=0; i<p_index.size(); i=i+1) {
					float angle = 0.0;
					for(int m = p_index[i]; m<(p_index[i]+11); m++) {
						angle = angle+0.02*an_g[m];
					}
			        angle = (float)(180*angle/3.1415927);  // convert into degrees
					step_angle_esti.push_back(angle);
				}
				// update the detection result
				Step_Count = Step_Count + p_index.size();
				cout << "### step angle: " << step_angle_esti[i] << endl;
				for(i=0; i<p_ts.size(); i++)	Step_Time.push_back(p_ts[i]);
				for(i=0; i<step_esti.size(); i++)	Step_Length.push_back(step_esti[i]);
				for(i=0; i<step_angle_esti.size(); i++)	Step_Heading.push_back(step_angle_esti[i]);
				E_flag = true;
						
			}
					
			// buffer in latest data
			tss1.clear();
			for(i=0; i<tss2.size(); i++)	tss1.push_back(tss2[i]);
			tss2.clear();
				
			a_g1.clear();
			for(i=0; i<a_g2.size(); i++)	a_g1.push_back(a_g2[i]);
			a_g2.clear();
					
			an_g1.clear();
			for(i=0; i<an_g2.size(); i++)	an_g1.push_back(an_g2[i]);
			an_g2.clear();
					
		}
    }
}

void* motion_sense(void*) {
	Pedometer p;
	p.run();
}
