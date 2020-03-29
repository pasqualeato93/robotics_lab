#include "ros/ros.h"
#include "esercizio3/service.h"
#include <iostream>
#include <sstream>

int one;
int two;
int sum;


using namespace std;

bool service_callback (esercizio3::service::Request &req, esercizio3::service::Response &res) {

 std::stringstream ss;
 one=atoi(req.numbera.c_str());

 cout <<" Number one  ----> " <<one<<endl;
 two=atoi(req.numberb.c_str());

 cout <<" Number two  ----> " <<two<<endl;
 sum=one+two; 

 ss<<sum;
 res.sum=ss.str();
 ROS_INFO("Server says [%s]", res.sum.c_str());
 cout<<endl;

 return true;
 
}
 

int main (int argc, char **argv){

 ros::init(argc, argv, "nodetwo");
 ros::NodeHandle n;
 ros::ServiceServer service= n.advertiseService("service", service_callback);
 ROS_INFO("Ready to receive from client.");
 ros::spin();

return 0;

}

