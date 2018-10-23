#pragma once
#include "stdafx.h"
#define VER_H7
struct Joint_jog {
	float j1; // J1 axis angle (radian)
	float j2; // J2 axis angle (radian)
	float j3; // J3 axis angle (radian)
	float j4; // J4 axis angle (radian)
	float j5; // J5 axis angle (radian)
	float j6; // J6 axis angle (radian)
	float j7; // Additional axis 1 (J7 axis angle) (radian)
	float j8; // Additional axis 2 (J8 axis angle) (radian)
};

struct XYZABC_jog {
	float x; // X axis coordinate value (mm)
	float y; // Y axis coordinate value (mm)
	float z; // Z axis coordinate value (mm)
	float a; // A axis coordinate value (radian)
	float b; // B axis coordinate value (radian)
	float c; // C axis coordinate value (radian)
	float finger; // Additional axis 1 (mm or radian)
	float nail; // Additional axis 2 (mm or radian)
};

struct Pose_jog {
	XYZABC_jog xyzabc_jog;
	uint32_t flag1; // Structural flag 1
	uint32_t flag2; // Structural flag 2
};

struct Pulse_jog {
	uint32_t p1; // Motor 1 axis
	uint32_t p2; // Motor 2 axis
	uint32_t p3; // Motor 3 axis
	uint32_t p4; // Motor 4 axis
	uint32_t p5; // Motor 5 axis
	uint32_t p6; // Motor 6 axis
	uint32_t p7; // Additional axis 1 (Motor 7 axis)
	uint32_t p8; // Additional axis 2 (Motor 8 axis)
};

enum Command_type : uint16_t {
	do_nothing = 0,
	move_to    = 1,
	stop_move  = 255,
};

enum Data_type : uint16_t {//much more options, only list the few we need
	no_data        = 0,
	xyz_data       = 1,	//command data
	joint_data     = 2,
	pulse_data     = 3,
	xyz_filter     = 4,
	joint_filter   = 5,
	pulse_filter   = 6,
	xyz_encoder    = 7,
	joint_encoder  = 8,
	pulse_encoder  = 9,
	current_command  = 10,
	current_feedback = 11,
	monitor_robot  = 12,
};

enum IO_type : uint16_t {
	no_io_data = 0,
	from_robot = 1,
	to_robot   = 2,
};

enum Acceleration_type : uint16_t {
	has_stopped		= 0,
	accelerating	= 1,
	constant_speed	= 2,
	decelerating	= 3,
};

struct Robot_information {
	float tool_point_speed_feedback{};	//unit: mm/s
	float remain_distance_feedback{};		//unit: mm
	float tool_point_speed_target{};		//unit: mm/s
	float remain_distance_target{};		//unit: mm
	float gap_btw_feedback_command{};		//unit: mm
	uint16_t control_factor{};			//no dimension
	Acceleration_type acceleration_type{ Acceleration_type::has_stopped };
	uint16_t step_number{};				//step number for BASIC program in Slot 1.
	char program_name[6]{};				//program name of the BASIC program in Slot 1.
	uint16_t controller_temperatur{};	//unit: 0.1 Celsius degree
	uint16_t not_used{};					//no meanning
	uint32_t monitring_counter{};			//step size is 7.1 ms.
};

union State { // Command data
	Pose_jog pose_jog;
	Joint_jog joint_jog;
	Pulse_jog pulse_jog;
	Robot_information robot_information;
	State():
		pose_jog()
	{}
};

struct Data {
	Command_type command_type;
	Data_type send_type;
	Data_type receive_type1;
	State	  send_receive_state{}; //also used to store send data
	IO_type   send_io_type;
	IO_type   receive_io_type;
	uint16_t  bit_top{};	//for IO
	uint16_t  bit_mask{};	//for IO
	uint16_t  IO_data{};	//for IO
	uint16_t  timeout_counter{};
	uint32_t  UDP_pack_index{};	//UDP pack index
	Data_type receive_type2;
	State	  receive_state2{};
	Data_type receive_type3;
	State	  receive_state3{};
	Data_type receive_type4;
	State	  receive_state4{};
	Data() : 
		command_type(Command_type::move_to),
		send_type(Data_type::xyz_data),
		receive_type1(Data_type::xyz_data),
		send_io_type(IO_type::no_io_data),
		receive_io_type(IO_type::no_io_data),
		receive_type2(Data_type::xyz_filter),
		receive_type3(Data_type::xyz_encoder),
		receive_type4(Data_type::joint_data)
	{}
};
class Robot_control {
public:
	Data send_data{};
	Data receive_data{};
private:
	char robot_ip_address[24];
	uint16_t port;
	SOCKADDR_IN robot_socket_address;
	SOCKET socket_handler{};
	WSADATA socket_information;
	int32_t socket_status;
	uint32_t UDP_counter;
	fd_set socket_set;
	timeval socket_time_out;

public:
	Robot_control();
	Robot_control(const Robot_control&) = delete;
	Robot_control& operator= (const Robot_control&) = delete;
	void Pack_send();
	void Pack_receive();
	int Robot_stop();
	~Robot_control();
private:
	inline void Form_first_send_data();
	inline void Form_stop_send_data();
	inline void Form_send_pack(char * send_pack);
	inline int Check_send(int byte_number_sent);
	inline int Check_receive(int byte_number_received);
	inline void Restore_received_data(char * receive_pack);
	inline void Sync_send_receive_data();
};
extern Robot_control * robot_control_ptr;