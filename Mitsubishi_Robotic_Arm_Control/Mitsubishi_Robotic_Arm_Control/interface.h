#pragma once
#include <cstdint>
//interface exposed to dll user
#ifdef MITSUBISHI_ROBOTIC_ARM_CONTROL_EXPORTS
#define control_api __declspec(dllexport)
#else
#define control_api __declspec(dllimport)
#endif

#pragma pack(push, 8)
	struct XYZ_jog
	{
		float x;	//unit: mm
		float y;	//unit: mm
		float z;	//unit: mm
		float a;	//unit: rad
		float b;	//unit: rad
		float c;	//unit: rad
	};
	struct Joint
	{
		float j1;	//unit: rad
		float j2;	//unit: rad
		float j3;	//unit: rad
		float j4;	//unit: rad
		float j5;	//unit: rad
		float j6;	//unit: rad
	};
#pragma pack(pop) 

	/*
	NOTICE:
	1. When any of below functions are called, remember check return values.
		If there is a negative return value, meaning an error has occured.
	2. In case any of these below functions returns a negative value, meaning control has failed.
		try call function "Init_robot" explicitly to fix the problem.
	3. One does't have to call function "Init_robot" explicitly while using the dll under normal circumstances.
		When dll starts up, function "DllMain" initilizes all needed resources automatically.
	4. One does't have to call function "Stop_robot" explicitly while using the dll under normal circumstances.
		When dll is closes up, function "DllMain" releases all resources used by the dll, including calling
		function "Stop_robot" to transit control to robot controller, meaning real-time control has ended.
	5. One only calls function "Init_robot" when trying recover from network failure.
	6. One only calls function "Stop_robot" when the robot needs to be stopped immediately while executing.
	*/

extern "C" {
	control_api int64_t  __cdecl Init_robot();
	control_api int64_t __cdecl CurrentPose(XYZ_jog* current_pose_ptr, Joint* current_joint_ptr);
	//get the current pose of the robotic arm
	control_api int64_t  __cdecl MoveTo(XYZ_jog move_to_pose, XYZ_jog* current_pose_ptr, Joint* current_joint_ptr);
	//move robotic arm to the position specified by the paramter move_to_pose
	//update the current position pointed by current_pose_ptr
	control_api int64_t  __cdecl Stop_robot();	//
}