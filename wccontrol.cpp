#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;


unsigned short calcSum(unsigned char *ptr){
	int n=0;
	unsigned short c=0;
	n=*(ptr++);
	n-=2;

	while(n>1){
		c+=(*(ptr)<<8) | *(ptr+1);
		c=c&0xffff;
		n-=2;
		ptr+=2;
	}

	if(n>0)
		c=c^(short)*(ptr++);
	if(c<0)
		c=(-1*c);
	return c;
}

int openSerial(char* port){
	int serial;
	struct termios options;
	
	//open the serial port for reading/writing and blocking reads. Do not take ownership.
	serial = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
	
	//check the serial port succesfully opened.
	if(serial==-1){
		std::cout << "failed to open " << port << std::endl;
		std::cout << "Exiting..." << std::endl;
		std::exit(1);
	}
	
	else{
//		if(instance.verbose)
			std::cout << "successfully opened " << port << std::endl;
		
		//get the attributes for the port
		tcgetattr(serial, &options);
		
		//specify attributes
		
		//baud rate 19200
		cfsetispeed(&options, B19200);
		cfsetospeed(&options, B19200);
		
		//various read/write options. Most important is blocking reads, raw input.
		options.c_cflag |= (CLOCAL | CREAD | CS8);
		options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		options.c_oflag &= ~OPOST;
		options.c_iflag |= (IXON | IXOFF | IXANY);
		
		tcsetattr(serial, TCSANOW, &options);
		
		//set the specified attributes
		fcntl(serial, F_SETFL, 0);
	}
	
	return serial;
}

int sendData(int port, unsigned char* data){
	//call the function to write a string to the serial port.
	return write(port, data, 12);
}

void updateStatus(int spd, int rtt, int port)
{
	unsigned char data[12];
	data[0] = 0xee;
	data[1] = 0xaa;
	data[8] = 0x01;

	if(spd>=0)
	{
		data[2] = 0x01;
		data[3] = (spd>>8)&0xff;
		data[4] = spd&0xff;
	}
	else
	{
		data[2] = 0x02;
		spd = -spd;
		data[3] = (spd>>8)&0xff;
		data[4] = spd&0xff;
		spd = -spd;
	}
	
	if(rtt>=0)
	{
		data[5] = 0x01;
		data[6] = (rtt>>8)&0xff;
		data[7] = rtt&0xff;
	}
	else
	{
		data[5] = 0x02;
		rtt = -rtt;
		data[6] = (rtt>>8)&0xff;
		data[7] = rtt&0xff;
		rtt = -rtt;
	}

	unsigned short chksum = 0;
	chksum = calcSum(data+2);
	data[9] = (chksum>>8)&0xff;
	data[10] = chksum&0xff;

	data[11] = 0xbb;
	
	sendData(port, data);
		
}

int wcopen() {
	char portfile[] = "/dev/ttyUSB0";
	int port = openSerial(portfile);
	return port;
}

int wccontrol(int speed, int rotation, int port)
{
	cout << speed << "\t" << rotation << endl;
	updateStatus(speed, rotation, port);
}
