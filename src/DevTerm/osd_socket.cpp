/*
	Skelton for retropc emulator

	Author : shikarunochi
	Date   : 2022/03/16

	[ dummy socket ]
*/

#include "osd.h"

void OSD::initialize_socket()
{

}

void OSD::release_socket()
{

}

void OSD::notify_socket_connected(int ch)
{

}

void OSD::notify_socket_disconnected(int ch)
{

}

void OSD::update_socket()
{

}

bool OSD::initialize_socket_tcp(int ch)
{
	return false;
}

bool OSD::initialize_socket_udp(int ch)
{
	return false;
}

bool OSD::connect_socket(int ch, uint32_t ipaddr, int port)
{
	return false;
}

void OSD::disconnect_socket(int ch)
{
}

bool OSD::listen_socket(int ch)
{
	return false;
}

void OSD::send_socket_data_tcp(int ch)
{
}

void OSD::send_socket_data_udp(int ch, uint32_t ipaddr, int port)
{
}

void OSD::send_socket_data(int ch)
{
}

void OSD::recv_socket_data(int ch)
{
}

