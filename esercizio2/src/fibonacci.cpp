#include "ros/ros.h"
#include "esercizio2/service.h"
#include <iostream>
#include <sstream>

using namespace std;

int number1=0;
int number2=0;
int i=0;

int main (int argc, char **argv){

   ros::init(argc, argv, "fibonacci");
   ros::NodeHandle n;
   ros::Rate loop_rate (10);
   ros::ServiceClient client= n.serviceClient<esercizio2::service>("service");

 while (ros::ok()) {
   esercizio2::service srv;
   std::stringstream ss;

   cout <<"Write first number : ";
   cin>>number1;
   cout <<"Write second number : ";
   cin>>number2;
   cout<<"Fibonacci :  ";
   i=0;

     do {
		
        ss<<number1;
        srv.request.in=ss.str();

         if(client.call(srv)) {
         cout<< srv.response.out << "  ";
         }

         else {
         ROS_ERROR ("Failed to call service");

         return 1; 
        }

     ss.str("");

     number1++;
     i++;
     }

   while (i<number2);

   cout<<endl;
   ros::spinOnce();
   loop_rate.sleep();

 }

   return 0;
}
