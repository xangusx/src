#ifndef _REMOTE_H_
#define _REMOTE_H_

#include "ros/ros.h"
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>

ros::Publisher car_pub;
ros::Subscriber car_sub;
ros::Publisher scara_pub;
ros::Subscriber scara_sub;
ros::Publisher pump_pub;
ros::Subscriber pump_sub;
ros::Publisher other_pub;
ros::Subscriber other_sub;

geometry_msgs::Twist vel_msg;
geometry_msgs::Point scara_msg;
geometry_msgs::Point pump_msg;
geometry_msgs::Point other_msg;

float max_vel = 0.35;
float turn_vel = 0.01;

void car_vel_callback(const geometry_msgs::Twist::ConstPtr& vel_data);
void scara_callback(const geometry_msgs::Point::ConstPtr& scara_data);
void pump_callback(const geometry_msgs::Point::ConstPtr& pump_data);
void other_callback(const geometry_msgs::Point::ConstPtr& other_data);

#endif