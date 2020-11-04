// STMマイコンにCANMessageのtopicを流す

#include <ros/ros.h>
#include <vector>
#include <can_controller.h>
#include <can_msgs/Frame.h>
#include <AKROS_bridge/Initialize.h>
#include <AKROS_bridge/motor_cmd.h>
#include <AKROS_bridge/motor_reply.h>
#include <std_srvs/Empty.h>

std::vector<can_msgs::Frame> can_msg;

// from ROS to STM
ros::Publisher  stm_pub;
ros::Subscriber ros_sub;
    
// from STM to ROS
ros::Publisher  ros_pub;
ros::Subscriber stm_sub;

// special CAN message
ros::ServiceServer enter_motor_control_mode_client;
ros::ServiceServer exit_motor_control_mode_client;
ros::ServiceServer reset_position_client;


// STMに流すcan_msgs
void ros2can_Cb(const AKROS_bridge::motor_cmd& cmd){
    std_msgs::UInt8MultiArray msg_;
    msg_.data.resize(TX_DATA_LENGTH);
    pack_cmd(&msg_, cmd.id, cmd.position, cmd.velocity, cmd.Kp, cmd.Kd, cmd.torque);
    stm_pub.publish(msg_);
}


// ROS側に流すmotor_reply
void can2ros_Cb(const can_msgs::Frame& can_reply_){
    uint8_t id_;
    float pos_, vel_, tor_;
    unpack_reply(msg, &id_, &pos_, &vel_, &tor_);

    AKROS_bridge::motor_reply_single reply_;
    reply_.id = id_;
    reply_.position = pos_;
    reply_.velocity = vel_;
    reply_.torque = tor_;

    ros_pub.publish(reply_);
}


// Enter motor control mode
void enter_control_motor_mode_Cb(const AKROS_bridge::Initialize::Request& req_, AKROS_bridge::Initialize::Response& res_){
    std_msgs::UInt8MultiArray cmd_;
    cmd_.data.resize(TX_DATA_LENGTH);
    for(int i=0; i<TX_DATA_LENGTH; i++){
        if(i == 0){
            cmd_.data[i] = id_->data;
        }else if(i < TX_DATA_LENGTH-1){
            cmd_.data[i] = 0xFF;
        }else{
            cmd_.data[i] = 0xFC;
        }
    }
    stm_pub.publish(cmd_);
}

// Exit motor control mode
void exit_control_motor_mode_Cb(const std_srvs::Empty::Request& req_, std_srvs::Empty::Response& res_){
    std_msgs::UInt8MultiArray cmd_;
    cmd_.data.resize(TX_DATA_LENGTH);
    for(int i=0; i<TX_DATA_LENGTH; i++){
        if(i == 0){
            cmd_.data[i] = id_->data;
        }else if(i < TX_DATA_LENGTH-1){
            cmd_.data[i] = 0xFF;
        }else{
            cmd_.data[i] = 0xFD;
        }
    }
    stm_pub.publish(cmd_);
}


// Reset motor position
void reset_motor_position_Cb(const std_srvs::Empty::Request& req_, std_srvs::Empty::Response& res_){
    std_msgs::UInt8MultiArray cmd_;
    cmd_.data.resize(TX_DATA_LENGTH);
    for(int i=0; i<TX_DATA_LENGTH; i++){
        if(i == 0){
            cmd_.data[i] = id_->data;
        }else if(i < TX_DATA_LENGTH - 1){
            cmd_.data[i] = 0xFF;
        }else{
            cmd_.data[i] = 0xFE;
        }
    }
    stm_pub.publish(cmd_);
}




int main(int argc, char** argv){
    ros::init(argc, argv, "motor_value_converter");
    ros::NodeHandle nh;


    // To STM
    stm_pub = nh.advertise<can_msgs::Frame>("motor_can_cmd", 10);  // STMマイコンにpub
    ros_sub = nh.subscribe("motor_cmd", 10, ros2can_Cb);
    
    // To ROS
    ros_pub = nh.advertise<AKROS_bridge::motor_reply_single>("motor_reply", 10);
    stm_sub = nh.subscribe("can_motor_reply", 10, can2ros_Cb);  // STMマイコンからsub

    // special CAN code
    enter_motor_control_mode_client = nh.subscribe<std_msgs::UInt8>("enter_motor_control_mode", 10, enter_control_motor_mode_Cb);
    exit_motor_control_mode_client = nh.subscribe<std_msgs::UInt8>("exit_motor_control_mode", 10, exit_control_motor_mode_Cb);
    reset_position_client = nh.subscribe<std_msgs::UInt8>("set_zero_position", 10, reset_motor_position_Cb);

    ROS_INFO("Converter Ready ...");

    while(ros::ok()){
        ros::spinOnce();
    }

    return 0;
}