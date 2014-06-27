#ifndef VFF_H
#define VFF_H

#define 	PI 					3.1415926
#define		Kp 					0.5

#define		true				1
#define		false				0

#define 	sonar_tracking		0
#define		vision_tracking		1
#define		sensor_num			9

#define 	dangerous			1000
#define 	stopdis				200
#define		MaxVSpeed			1000
#define		MaxWSpeed			90			
#define		vect_max			700
#define		vect_infinit		2000

typedef		struct 
{
	int 		dis;
	float		vect;
	float		angle;
	/* data */
}Sensor;
typedef		struct
{
	int 	v;
	int 	w;
	/* data */
}COMMAND;


COMMAND vff(int pos[2], int data[9], int speed[]);

#endif
