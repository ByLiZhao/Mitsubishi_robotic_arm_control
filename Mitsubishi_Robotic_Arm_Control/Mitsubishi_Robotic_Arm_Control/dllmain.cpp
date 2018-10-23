// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "interface.h"
#include "robotic_arm_control.h"
//Robot_control * robot_control_ptr;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Init_robot();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Stop_robot();
		break;
	}
	return TRUE;
}

