// robotic_arm_control.cpp : Defines the exported functions for the DLL application.
#include "stdafx.h"
#pragma comment (lib, "Ws2_32.lib")
#include "robotic_arm_control.h"

#define NO_FLAG 0
#define BUF_SIZE 512
#define robot_ip_address_default "192.168.0.20"
#define port_default 10000
Robot_control:: Robot_control() {
	//init private member
	strcpy(robot_ip_address, robot_ip_address_default);
	port = port_default;
	robot_socket_address.sin_addr.s_addr = inet_addr(robot_ip_address);
	robot_socket_address.sin_port = htons(port);
	robot_socket_address.sin_family = AF_INET;
	socket_time_out.tv_sec = 1;
	socket_time_out.tv_usec = 0;
	UDP_counter = 0;
	// Windows Socket DLL initialization
	socket_status = WSAStartup(MAKEWORD(1, 1), &socket_information);
	if (socket_status != 0) {
		throw std::exception("WSAStartup unsuccessful.\n");
	}
		// Socket creation
	socket_handler = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_handler == INVALID_SOCKET) {
		throw std::exception("Socket initialization unsuccessful\n");
	}
	FD_ZERO(&socket_set);
	FD_SET(socket_handler, &socket_set);
	Form_first_send_data();
	Pack_send();
	Pack_receive();
	Sync_send_receive_data();
	//restore altered settings
	//socket_time_out.tv_sec = 1;
	//socket_time_out.tv_usec = 20000;
	send_data.command_type = Command_type::move_to;
	send_data.send_type = Data_type::xyz_data;
	send_data.receive_type1 = Data_type::xyz_data;
	send_data.receive_type2 = Data_type::xyz_filter;
	send_data.receive_type3 = Data_type::xyz_encoder;
	send_data.receive_type4 = Data_type::joint_data;
	}
int Robot_control::Robot_stop() {
	send_data.UDP_pack_index = UDP_counter;
	char send_pack[BUF_SIZE] = {};
	Form_stop_send_data();
	Form_send_pack(send_pack);
	int byte_number_sent = sendto(socket_handler, send_pack, sizeof(Data), NO_FLAG, (LPSOCKADDR)&robot_socket_address, sizeof(robot_socket_address));
	if (Check_send(byte_number_sent) == -1) {
		return -1;
	}
	socket_status = select(0, &socket_set, nullptr, nullptr, &socket_time_out);
	if (socket_status == SOCKET_ERROR) {
		return -1;
	}
	if (socket_status > 0) {
		char receive_pack[BUF_SIZE] = {};
		int byte_number_received = recvfrom(socket_handler, receive_pack, BUF_SIZE, NO_FLAG, nullptr, nullptr);
		//std::cout << "byte number received is" << byte_number_received << std::endl;
		int temp_count = 1;
		while (Check_receive(byte_number_received) == -1 && temp_count <= 10)
		{
			byte_number_received = recvfrom(socket_handler, receive_pack, BUF_SIZE, NO_FLAG, nullptr, nullptr);
			temp_count++;
		}
		//Restore_received_data(receive_pack);
		//UDP_counter++;
		return Check_receive(byte_number_received);
	}
	else
	{
		return -1;
	}
	}
void Robot_control::Pack_send() {
	send_data.UDP_pack_index = UDP_counter;
	char send_pack[BUF_SIZE] = {};
	Form_send_pack(send_pack);
	int byte_number_sent = sendto(socket_handler, send_pack, sizeof(Data), NO_FLAG, (LPSOCKADDR)&robot_socket_address, sizeof(robot_socket_address));
	while (Check_send(byte_number_sent) == -1)
	{
		throw std::exception("in function Pack_send: sendto function unsuccessful");
	}
}
void  Robot_control:: Pack_receive() {
	socket_status = select(0, &socket_set, nullptr, nullptr, &socket_time_out);
	if (socket_status == SOCKET_ERROR) {
		throw std::exception("in function Pack_receive: Socket select function unsuccessful");
	}
	//if ((socket_status > 0) && (FD_ISSET(socket_handler, &socket_set) != 0)) {
	if (socket_status > 0) {
		char receive_pack[BUF_SIZE] = {};
		int byte_number_received = recvfrom(socket_handler, receive_pack, BUF_SIZE, NO_FLAG, nullptr, nullptr);
		//std::cout << "byte number received is" << byte_number_received << std::endl;
		int temp_count = 1;
		while (Check_receive(byte_number_received) == -1 && temp_count<=10)
		{
			byte_number_received = recvfrom(socket_handler, receive_pack, BUF_SIZE, NO_FLAG, nullptr, nullptr);
			temp_count++;
		}
		Restore_received_data(receive_pack);
		UDP_counter++;
	}
	else { 
		throw std::exception("in function Pack_receive: Socket receive timeout");
	}
}
Robot_control :: ~Robot_control() {
	socket_status = closesocket(socket_handler);      //close socket
	if (socket_status == SOCKET_ERROR) {
		std::cerr << "closesocket unsuccessful" << std::endl;;
	}
	socket_status = WSACleanup();
	if (socket_status == SOCKET_ERROR) {
		std:: cerr<< "WSACleanup unsuccessful"<<std::endl;
	}
}
inline void Robot_control:: Form_first_send_data() {
	send_data.command_type = Command_type::do_nothing;	
	send_data.send_type = Data_type::no_data;
	send_data.send_io_type = IO_type::no_io_data;
	send_data.receive_io_type = IO_type::no_io_data;
}
inline void Robot_control::Form_stop_send_data() {
	send_data.command_type = Command_type::stop_move;
	send_data.send_type = Data_type::no_data;
}
inline void Robot_control::Form_send_pack(char * send_pack) {
	memcpy(send_pack, (char*)&send_data, sizeof(Data));
}
inline int Robot_control:: Check_send(int byte_number_sent) {
	if (byte_number_sent != sizeof(Data)) {
		return -1;
	}
	return 0;
}
inline int Robot_control::Check_receive(int byte_number_received) {
	if (byte_number_received == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}
inline void Robot_control:: Restore_received_data(char * receive_pack) {
	memcpy((char *)&receive_data, receive_pack, sizeof(Data));
}
inline void Robot_control:: Sync_send_receive_data() {
	send_data = receive_data;
}