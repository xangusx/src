#ifndef _SCARA_H_
#define _SCARA_H_

#include "ros/ros.h"
#include "detect_square.h"
#include <geometry_msgs/Point.h>

ros::Publisher scara_pub;
// int remainder = 3;
int numberofsquare;

void ScaraTake();
bool Scara_move(float x,float y,int init);

#endif