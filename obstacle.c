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
#include <math.h>

#define FALSE -1
#define TRUE 0

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

int openrobot()
{
  printf("This program updates last time at %s   %s\n",__TIME__,__DATE__);
  printf("STDIO COM1\n");
  int fd;
  fd = open("/dev/ttyUSB0",O_RDWR);
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

  set_speed(fd,9600);
  if (set_Parity(fd,8,1,'N') == FALSE)  {
    printf("Set Parity Error\n");
    exit (0);
  }
	return fd;
}

void readrobot(int port) {
	char raw_buf[512];
	int data[512];
	char b;
	int nread;
	int x, y, theta, i, j, k, p, q;
	int state = 0;
	int num_len = 0;
	int buf_len = 0;
	char buf[128];
	int start_pos, end_pos;
	int prev_pos;
	while(1) {
		if((nread=read(port, raw_buf, 512)) > 0) {
			for(i=0; i<nread; i++) {
				b = raw_buf[i];
				if(b=='\n') {
					prev_pos = 0;
					for(j=0; j<=buf_len; j++) {
						if(buf[j] == ' ' || j == buf_len) {
							num_len = j-prev_pos;
							x=0;
							for(k=prev_pos, p=num_len-1; k<j; k++, p--) {
								y = buf[k]-'0';
								for(q=0; q<p; q++) y*=10;
								x+=y;
							}
							printf("%d, ", x);
							prev_pos = j+1;
						}
					}
					buf_len = 0;
					printf("\n");
				} else {
					buf[buf_len] = b;
					buf_len++;
				}
			}
			memset(raw_buf, 0, sizeof(raw_buf));
		}
	}
}

void main() {
	int port = openrobot();
	readrobot(port);
}
