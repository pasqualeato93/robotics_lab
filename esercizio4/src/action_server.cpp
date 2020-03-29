#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <actionlib/server/simple_action_server.h>
#include "esercizio4/demoAction.h"
#include <iostream>
#include <sstream>

class action_class {

   private:

     ros::NodeHandle nh_;
     actionlib::SimpleActionServer<esercizio4::demoAction> as; 
     esercizio4::demoFeedback feedback;
     esercizio4::demoResult result;

     std::string action_name;
     int goal;
     int progress;
     int speed=0;
     int sum=0;

   public:

    action_class(std::string name) :
    as(nh_, name, boost::bind(&action_class::executeCB, this, _1), false),
    action_name(name) {
    as.registerPreemptCallback(boost::bind(&action_class::preemptCB, this));
    as.start();
}
	
void preemptCB(){

   ROS_WARN("%s got preempted!", action_name.c_str());
   result.pos_finish = progress;
   as.setPreempted(result,"I got Preempted"); 

}
  


void executeCB(const esercizio4::demoGoalConstPtr &goal) {

   if(!as.isActive() || as.isPreemptRequested()) return;

   ros::Rate rate(10);
   ROS_INFO("%s is processing the goal %d", action_name.c_str(), goal->pos_end);

   for(progress = goal->pos_start ; progress <= goal->pos_end; progress++){
				
      progress--;
				
        if (!ros::ok()) {

           result.pos_finish = progress;
           as.setAborted(result,"I failed !");
           ROS_INFO("%s Shutting down",action_name.c_str());
  
           break;
        }
		
        if(!as.isActive() || as.isPreemptRequested()){

           return;
        }	
				
     sum=0;

     for(int i=0; i<speed;i++){

       sum=sum+i;
     }

     if (sum>=goal->pos_end-progress && speed>=1){

       speed--;
     }

     else{

      speed++;

         if (speed>goal->velocity){
         speed=speed-1;
         }
     }
			
   progress=progress+speed;

   if(goal->pos_end <= progress) {

     ROS_INFO("Getting to goal %d with velocity %d", progress, speed);
     result.pos_finish = progress;
     as.setSucceeded(result);
   }

   else {

     feedback.current_pos = progress;
     ROS_INFO("Setting to goal %d / %d   velocity -> %d",feedback.current_pos,goal->pos_end,speed);				
     as.publishFeedback(feedback);
   } 

  rate.sleep();
  }

}
	
};



int main(int argc, char** argv){

  ros::init(argc, argv, "demo_action");
  ROS_INFO("Starting Demo Action Server");
  action_class demo_action_obj(ros::this_node::getName());
  ros::spin();

  return 0;
}

