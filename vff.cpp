#include "vff.h"
#include <math.h>


/*
 *pos: d, theta
 *data: ultra sonic input
 *speed: n/a
*/
COMMAND vff(int pos[2], int data[9], int speed[])
{
	int 	safe = true, stop = 0, index = 0;
	int 	cnt = 0;
	Sensor  sensor[sensor_num];
	float	vect_x, vect_y, vect_theta;		// robot frame 
	COMMAND speed_cmd;

	vect_x = 0;
	vect_y = 0;
	vect_theta = 0;
	//fill data
	for(index=0; index<sensor_num; index++)
	{
		sensor[index].dis = data[index];
		sensor[index].vect = 0;
	}
	
	//set angle 
	sensor[0].angle = -90;
	sensor[8].angle = 90;
	for(index = 0; index < 7; index++ )
	{
		sensor[index+1].angle = -45 + index * 15;
	}

	//testify environment 
	for(index = 0; index < sensor_num; index++ )
	{
		if( sensor[index].dis < dangerous )
		{
			safe = false;
			if( sensor[index].dis < stopdis )
				stop ++;
		}
	}
	// if environment is not safe, caculate safe command
	if( false == safe )
	{
		// if environment is too dangerous, just stop the car
		if( stop > 7)
		{
			speed_cmd.v = 0;
			speed_cmd.w = 0;
		}
		// else caculate safe cmd
		else
		{
			//first caculate vector for each sensor
			// sencond, caculate vector sum	
			for(index=0; index<sensor_num; index++)
			{

				if( sensor[index].dis < dangerous && sensor[index].dis )
				{
					sensor[index].vect = (dangerous - sensor[index].dis)*1.0/(dangerous - stopdis) * (dangerous - sensor[index].dis)*1.0/(dangerous - stopdis) * vect_max;
					cnt++;
				}
				else if(sensor[index].dis < stopdis )
				{
					sensor[index].vect = vect_infinit;
					cnt++;
				}
				else
				{
					sensor[index].vect = 0;
				}
				vect_x += sensor[index].vect * cos((sensor[index].angle - 180)/180*PI);
				vect_y += sensor[index].vect * sin((sensor[index].angle - 180)/180*PI);
			}
			
			vect_x = vect_x/cnt * Kp + vect_max * cos( pos[1]/180.0*PI ) *(1-Kp);
			vect_y = vect_y/cnt * Kp + vect_max * sin( pos[1]/180.0*PI ) *(1-Kp);
			speed_cmd.v = sqrt(vect_x*vect_x + vect_y*vect_y);
			speed_cmd.w = asin( vect_y/(vect_x*vect_x + vect_y*vect_y))/PI * 180;

			if(vect_x>=0 && vect_y>=0)
			{
				
			}
			else if(vect_x>=0 && vect_y<0)
			{
				
			}
			else if(vect_y>=0 && vect_x<0)
			{
				speed_cmd.w = 90;
			}
			else if (vect_y<0 && vect_x<0)
			{
				speed_cmd.w = -90;
			}
			
		}
			
	}
	// else just send tracking speed_cmd
	else
	{
		speed_cmd.v = speed[0];
		speed_cmd.w = speed[1];
	}
	return speed_cmd;
}

