#include "stdafx.h"
#include "cmath"
#include "robotic_arm_control.h"
#include "interface.h"
float TOLL = 1.3f;
float ANGTOLL = 0.001f;
Robot_control * robot_control_ptr=nullptr;
int64_t  Init_robot() {
	try {
		if (robot_control_ptr != nullptr) {
			delete robot_control_ptr;
		}
		robot_control_ptr = new Robot_control();
		return 1;
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
}
int64_t  Send(XYZ_jog move_to_pose) {
	try {

		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.x = move_to_pose.x;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.y = move_to_pose.y;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.z = move_to_pose.z;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.a = move_to_pose.a;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.b = move_to_pose.b;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.c = move_to_pose.c;
		robot_control_ptr->Pack_send();
		return 1;
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
}

int64_t  Receive(XYZ_jog* current_pose_ptr) {
	try {
		robot_control_ptr->Pack_receive();
		current_pose_ptr->x = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.x;
		current_pose_ptr->y = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.y;
		current_pose_ptr->z = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.z;
		current_pose_ptr->a = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.a;
		current_pose_ptr->b = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.b;
		current_pose_ptr->c = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.c;
		/*std::cout << "-------------------------" << std::endl;
		std::cout << robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.x << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.y << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.z << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.a << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.b << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.c << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.finger << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.nail << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.flag1 << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.flag2 << ", "
		<< std::endl;
		std::cout << robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.x << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.y << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.z << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.a << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.b << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.c << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.finger << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.nail << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.flag1 << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.flag2 << ", "
		<< std::endl;
		std::cout << robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.x << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.y << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.z << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.a << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.b << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.c << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.finger << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.nail << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.flag1 << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.flag2 << ", "
		<< std::endl;
		std::cout << robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.x << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j1 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j2 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j3 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j4 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j5 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j6 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j7 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j8 << ", "
		<< std::endl;*/
		return 1;
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
}
inline float sign(float para){
	if (para > 0.001f)
		return 1.0f;
	else if(para < -0.001f)
		return -1.0f;
	else
		return 0.0f;
}
inline float delta(float a, float b) {
	return (b - a);
}
inline XYZ_jog CurrentPose()
{
	XYZ_jog current_pose;
	current_pose.x = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.x;
	current_pose.y = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.y;
	current_pose.z = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.z;
	current_pose.a = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.a;
	current_pose.b = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.b;
	current_pose.c = robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.c;
	return current_pose;
}
inline Joint CurrentJoint()
{
	Joint current_joint;
	current_joint.j1 = robot_control_ptr->receive_data.receive_state4.joint_jog.j1;
	current_joint.j2 = robot_control_ptr->receive_data.receive_state4.joint_jog.j2;
	current_joint.j3 = robot_control_ptr->receive_data.receive_state4.joint_jog.j3;
	current_joint.j4 = robot_control_ptr->receive_data.receive_state4.joint_jog.j4;
	current_joint.j5 = robot_control_ptr->receive_data.receive_state4.joint_jog.j5;
	current_joint.j6 = robot_control_ptr->receive_data.receive_state4.joint_jog.j6;
	return current_joint;
}
int64_t CurrentPose(XYZ_jog* current_pose_ptr, Joint* current_joint_ptr) {
	try {
		XYZ_jog current_pose = CurrentPose();
		Joint current_joint = CurrentJoint();
		*current_pose_ptr = current_pose;
		*current_joint_ptr = current_joint;
		return 1;
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
}
int64_t  MoveTo(XYZ_jog move_to_pose, XYZ_jog* current_pose_ptr, Joint* current_joint_ptr)
{
	XYZ_jog current_pose = CurrentPose();
	Joint current_joint = CurrentJoint();
	XYZ_jog move_to_pose_new;
	float delta_x = delta(current_pose.x, move_to_pose.x);
	if (fabs(delta_x) <= TOLL){
		move_to_pose_new.x = move_to_pose.x;
	}
	else {
		move_to_pose_new.x = (current_pose.x) + sign(delta_x) * TOLL;
	}
	//
	float delta_y = delta(current_pose.y, move_to_pose.y);
	if (fabs(delta_y) <= TOLL) {
		move_to_pose_new.y = move_to_pose.y;
	}
	else {
		move_to_pose_new.y = (current_pose.y) + sign(delta_y) * TOLL;
	}
	//
	float delta_z = delta(current_pose.z, move_to_pose.z);
	if (fabs(delta_z) <= TOLL) {
		move_to_pose_new.z = move_to_pose.z;
	}
	else {
		move_to_pose_new.z = (current_pose.z) + sign(delta_z) * TOLL;
	}
	//attention
		move_to_pose_new.a = move_to_pose.a;
		move_to_pose_new.b = move_to_pose.b;
		move_to_pose_new.c = move_to_pose.c;
	//send move to command
	int64_t send_result;
	try {

		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.x = move_to_pose_new.x;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.y = move_to_pose_new.y;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.z = move_to_pose_new.z;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.a = move_to_pose_new.a;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.b = move_to_pose_new.b;
		robot_control_ptr->send_data.send_receive_state.pose_jog.xyzabc_jog.c = move_to_pose_new.c;
		robot_control_ptr->Pack_send();
		send_result = 1;
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		send_result = -1;
	}
	//receive current position
	int64_t receive_result;
	try {
		robot_control_ptr->Pack_receive();
		current_pose = CurrentPose();
		current_joint = CurrentJoint();
		*current_pose_ptr = current_pose;
		*current_joint_ptr = current_joint;
		/*std::cout << "size of Data is" << sizeof(Data) << std::endl;
		std::cout << "size of  state is" << sizeof(State) << std::endl;
		std::cout << "-------------------------" << std::endl;
		std::cout << robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.x << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.y << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.z << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.a << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.b << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.c << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.finger << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.xyzabc_jog.nail << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.flag1 << ", "
		<< robot_control_ptr->receive_data.send_receive_state.pose_jog.flag2 << ", "
		<< std::endl;
		std::cout << robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.x << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.y << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.z << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.a << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.b << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.c << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.finger << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.xyzabc_jog.nail << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.flag1 << ", "
		<< robot_control_ptr->receive_data.receive_state2.pose_jog.flag2 << ", "
		<< std::endl;
		std::cout << robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.x << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.y << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.z << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.a << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.b << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.c << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.finger << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.nail << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.flag1 << ", "
		<< robot_control_ptr->receive_data.receive_state3.pose_jog.flag2 << ", "
		<< std::endl;
		std::cout << robot_control_ptr->receive_data.receive_state3.pose_jog.xyzabc_jog.x << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j1 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j2 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j3 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j4 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j5 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j6 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j7 << ", "
		<< robot_control_ptr->receive_data.receive_state4.joint_jog.j8 << ", "
		<< std::endl;*/
		receive_result = 1;
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		receive_result = -1;
	}
	// check if both send and receive succeed
	if (send_result == 1 && receive_result == 1)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}
int64_t  Stop_robot() {
	robot_control_ptr->Robot_stop();
	if (robot_control_ptr != nullptr) {
		delete robot_control_ptr;
	}
		return 1;
}