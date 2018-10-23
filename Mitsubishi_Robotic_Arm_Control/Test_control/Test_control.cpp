// Test_control.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdint>
#include <chrono>
#include <ctime>
#include "..\Mitsubishi_Robotic_Arm_Control\interface.h"

int main()
{
	#define Pi (3.14159)
	XYZ_jog another_pose;
	Joint current_joint;
	CurrentPose(&another_pose, &current_joint);
	XYZ_jog current_pose{};
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();	
	int64_t return_value = MoveTo(another_pose, &current_pose, &current_joint);
	for (size_t j = 0; j < 1; j++)
	{
		for (size_t i = 0; i < 100; i++)
		{
			another_pose.z += 3.0;
			another_pose.x += 1.0;
			another_pose.y += 1.0;
			return_value = MoveTo(another_pose, &current_pose, &current_joint);
		}
		Init_robot();
		for (size_t i = 0; i < 100; i++)
		{
			another_pose.z -= 1.0;
			another_pose.x -= 1.0;
			another_pose.y -= 1.0;
			return_value = MoveTo(another_pose, &current_pose, &current_joint);
		}
	}
	Stop_robot();
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    return 0;
}

