#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include <iostream>
#include <fstream>  

//Se avessi voluto farlo con un messaggio personalizzato, come avrei potuto fare il file .h?

void base_scanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{

  ros::Rate loop_rate(10);
  ros::NodeHandle n2;
  int i=0;
  float anglemax=msg->angle_max;
  float anglemin=msg->angle_min;
  float angleinc=msg->angle_increment;
  float length=(anglemax-anglemin)/angleinc;
  int intlength= int(length)+1;
/*ROS_INFO("angle_max= %f",anglemax);
  ROS_INFO("angle_min= %f",anglemin);
  ROS_INFO("angleinc= %f",angleinc);
  ROS_INFO("(angle_max-angle_min)/angle_increment= %f",length);
  */
  float min_distance=msg->ranges[0];
  for (i=0; i<intlength;i++){
    if(min_distance>msg->ranges[i]){
     min_distance=msg->ranges[i];
    }
  }

  std_msgs::Float32 msg2;

  ros::Publisher minDistance_pub = n2.advertise<std_msgs::Float32>("min_distance", 1000);

  msg2.data= min_distance;

  minDistance_pub.publish(msg2);

  ros::spinOnce();

  loop_rate.sleep();

  ROS_INFO("MINIMUM DISTANCE: %f", msg2.data);

  std::ofstream outfile ("min_distance.txt");

  outfile << msg2.data << std::endl;

  outfile.close();
  
//ROS_INFO("Ranges[%d]: [%f], Intensities[%d]: [%f] ",i, msg->ranges[i],i,msg->intensities[i]);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "homework1");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("base_scan", 1000, base_scanCallback);

  ros::spin();

  return 0;
}
