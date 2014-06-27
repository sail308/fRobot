#ifndef WCCONTROL_H
#define WCCONTROL_H

unsigned short calcSum(unsigned char *ptr);
int openSerial(char* port);
int sendData(int port, unsigned char* data);
void updateStatus(int spd, int rtt, int port);
int wcopen();
int wccontrol(int speed, int rotation, int port);

#endif
