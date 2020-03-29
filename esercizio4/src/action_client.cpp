#include "ros/ros.h"
#include <iostream>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "esercizio4/demoAction.h"

int times=0;

int main (int argc, char **argv) {

   ros::init(argc, argv, "demo_action_client");

   actionlib::SimpleActionClient<esercizio4::demoAction> ac("demo_action", true);

   ROS_INFO("Waiting for action server to start.");

   ac.waitForServer();

   ROS_INFO("Action server started, sending goal.");
  
   esercizio4::demoGoal goal;
  
   std::cout<<"Initial position : ";
   std::cin>>goal.pos_start;
   std::cout<<"Final position : ";
   std::cin>>goal.pos_end;
   std::cout<<"Speed max rpm : ";
   std::cin>>goal.velocity;
   std::cout<<"Time : ";
   std::cin>>times;

   ROS_INFO("Starter [%d] , Finish [%d] , Velocity [%d] , Time [%d]", goal.pos_start, goal.pos_end, goal.velocity, times);
  
   ac.sendGoal(goal);

  bool finished_before_timeout = ac.waitForResult(ros::Duration(times));

  ac.cancelGoal();

    if (finished_before_timeout) {

      actionlib::SimpleClientGoalState state = ac.getState();
      ROS_INFO("Action finished: %s",state.toString().c_str());

      ac.cancelGoal();
    }

   else

   ROS_INFO("Action did not finish before the time out.");

return 0;

}

