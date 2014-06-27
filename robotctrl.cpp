#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#include "robotctrl.h"

#define FALSE -1
#define TRUE 0

int g_robot_x, g_robot_y, g_robot_theta;

int speed_arr[] = { B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = { 115200, 38400,  19200,  9600,  4800,  2400,  1200,  300, 38400, 19200, 9600, 4800, 2400, 1200,  300, };
void set_speed(int fd, int speed){
  int   i; 
  int   status; 
  struct termios   Opt;
  tcgetattr(fd, &Opt); 
  for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) { 
    if  (speed == name_arr[i]) {     
      tcflush(fd, TCIOFLUSH);     
      cfsetispeed(&Opt, speed_arr[i]);  
      cfsetospeed(&Opt, speed_arr[i]);   
      status = tcsetattr(fd, TCSANOW, &Opt);  
      if  (status != 0) {        
        perror("tcsetattr fd1");  
        return;     
      }    
      tcflush(fd,TCIOFLUSH);   
    }  
  }
}

int set_Parity(int fd,int databits,int stopbits,int parity)
{ 
  struct termios options; 
  if  ( tcgetattr( fd,&options)  !=  0) { 
    perror("SetupSerial 1");     
    return(FALSE);  
  }
  options.c_cflag &= ~CSIZE; 
  options.c_lflag &= ~ECHO; //disable echo 
  //options.c_iflag |= TGNCR;
  switch (databits) 
  {   
  case 7:   
    options.c_cflag |= CS7; 
    break;
  case 8:     
    options.c_cflag |= CS8;
    break;   
  default:    
    fprintf(stderr,"Unsupported data size\n"); return (FALSE);  
  }
  switch (parity) 
  {   
    case 'n':
    case 'N':    
      options.c_cflag &= ~PARENB;   /* Clear parity enable */
      options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
      break;  
    case 'o':   
    case 'O':     
      options.c_cflag |= (PARODD | PARENB); 
      options.c_iflag |= INPCK;             /* Disnable parity checking */ 
      break;  
    case 'e':  
    case 'E':   
      options.c_cflag |= PARENB;     /* Enable parity */    
      options.c_cflag &= ~PARODD;    
      options.c_iflag |= INPCK;       /* Disnable parity checking */
      break;
    case 'S': 
    case 's':  /*as no parity*/   
        options.c_cflag &= ~PARENB;
      options.c_cflag &= ~CSTOPB;break;  
    default:   
      fprintf(stderr,"Unsupported parity\n");    
      return (FALSE);  
    }  
  
  switch (stopbits)
  {   
    case 1:    
      options.c_cflag &= ~CSTOPB;  
      break;  
    case 2:    
      options.c_cflag |= CSTOPB;  
       break;
    default:    
       fprintf(stderr,"Unsupported stop bits\n");  
       return (FALSE); 
  } 
  /* Set input parity option */ 
  if (parity != 'n')   
    options.c_iflag |= INPCK; 
  tcflush(fd,TCIFLUSH);
  options.c_cc[VTIME] = 0; 
  options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
  if (tcsetattr(fd,TCSANOW,&options) != 0)   
  { 
    perror("SetupSerial 3");   
    return (FALSE);  
  } 
  return (TRUE);  
}

int openrobot(const char* devfile, int baud)
{
  printf("This program updates last time at %s   %s\n",__TIME__,__DATE__);
  printf("STDIO COM1\n");
  int fd;
  fd = open(devfile,O_RDWR);
  if(fd == -1)
  {
    perror("serialport error\n");
  }
  else
  {
    printf("open ");
    printf("%s",ttyname(fd));
    printf(" succesfully\n");
  }

  set_speed(fd,baud);
  if (set_Parity(fd,8,1,'N') == FALSE)  {
    printf("Set Parity Error\n");
    exit (0);
  }
	return fd;
}

void controlrobot(int spd, int rtt, int port){
	unsigned char data[12] = {0xee, 0xaa, 0x01, 0x00, 0xff, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0xbb};

	if(spd>=0) {
		data[2] = 0x01;
	} else {
		data[2] = 0x02;
		spd = -spd;
	}
	data[3] = (spd>>8)&0xff;
	data[4] = spd&0xff;

	if(rtt>=0)
	{
		data[5] = 0x01;
	}
	else
	{
		data[5] = 0x02;
		rtt = -rtt;
	}
	data[6] = (rtt>>8)&0xff;
	data[7] = rtt&0xff;

	printf("Write %d bits\n", (int)write(port, data, sizeof(data)));
}

void readrobot(int port) {
	char buf[512];
	int nread, i;
	char data_buf[32];
	int data_len = 0;
	int d;
	printf("port = %d\n", port);
	while(1) {
		if((nread=read(port, buf, 512)) > 0) {
			for(i=0; i<nread; i++) {
				data_buf[data_len] = (unsigned char)buf[i];
				printf("%d\n", (unsigned char)buf[i]);
//				data_len++;
//				if(data_len == 32) {
					
//				}
			}
/*			g_robot_x = ((int)buf[2]<<8) | buf[3];
			g_robot_y = ((int)buf[4]<<8) | buf[5];
			g_robot_theta = ((int)buf[6]<<8) | buf[7];
			printf("%d %d %d", g_robot_x, g_robot_y, g_robot_theta);
*/			memset(buf, 0, sizeof(buf));
		}
	}
}

