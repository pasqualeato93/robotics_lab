#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <actionlib/server/simple_action_server.h>
#include "esercizio4/demoAction.h"
#include <iostream>
#include <sstream>

class action_class {

	private:
  	ros::NodeHandle nh_;
  	// NodeHandle instance must be created before this line. Otherwise strange error may occur.
  	actionlib::SimpleActionServer<esercizio4::demoAction> as; 
  	// create messages that are used to published feedback/result
		esercizio4::demoFeedback feedback;
		esercizio4::demoResult result;

  	std::string action_name;
  	int goal;
  	int progress;

	public:
		action_class(std::string name) :
  	  as(nh_, name, boost::bind(&action_class::executeCB, this, _1), false),
  	  action_name(name) {
				as.registerPreemptCallback(boost::bind(&action_class::preemptCB, this));
				as.start();
  		}
	
		void preemptCB(){
			ROS_WARN("%s got preempted!", action_name.c_str());
			result.end_pos = progress;
			as.setPreempted(result,"I got Preempted"); 
  	}
  
		void executeCB(const esercizio4::demoGoalConstPtr &goal) {
			if(!as.isActive() || as.isPreemptRequested()) return;
			ros::Rate rate(5);
			ROS_INFO("%s is processing the goal %d", action_name.c_str(), goal-> final_pos);
			for(progress = 1 ; progress <= goal->final_pos; progress++){
				//Check for ros
				if (!ros::ok()) {
					result.end_pos = progress;
					as.setAborted(result,"I failed !");
					ROS_INFO("%s Shutting down",action_name.c_str());
					break;
				}
		
				if(!as.isActive() || as.isPreemptRequested()){
					return;
				}	

				if(goal->final_pos <= progress) {
					ROS_INFO("%s Succeeded at getting to goal %d", action_name.c_str(), goal->final_pos);
					result.end_pos = progress;
					as.setSucceeded(result);
				}
				else {
					ROS_INFO("Setting to goal %d / %d",feedback.current_pos,goal->start_pos);
					feedback.current_pos = progress;
					as.publishFeedback(feedback);
			}
		rate.sleep();
		}	
  }
};

int main(int argc, char** argv)
{

  ros::init(argc, argv, "demo_action");
  ROS_INFO("Starting Demo Action Server");
  action_class demo_action_obj(ros::this_node::getName());
  ros::spin();
  return 0;
}
