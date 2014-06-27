#ifndef ROBOTCTRL_H
#define ROBOTCTRL_H


int openrobot(const char*, int);
void controlrobot(int spd, int rtt, int port);
void readrobot(int port);

#endif
