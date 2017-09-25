#pragma once

#include "stdafx.h"
#include <winsock2.h>

#define PORT_HTTP		80

//------------------------------------------------------------------------------------------------------
class wsa
{
public:
	typedef int error;

	static void init( _out WSADATA &data );
	static void cleanup();
	static error get_error();

	wsa();
	~wsa();

private:
	WSADATA m_data;
};

//------------------------------------------------------------------------------------------------------
class socket
{
public:
	class address
	{
	public:
		typedef byte_t ipv4_t[4];
		//typedef in_addr ipv4_t;
		typedef unsigned short port_t;
		
		address( _in ipv4_t host, _in port_t port = PORT_HTTP );
		address( _in cstr_t host, _in port_t port = PORT_HTTP );

		operator const sockaddr_in&() const;
		operator const sockaddr&() const;

	private:
		sockaddr_in m_data;
	};

	socket();
	~socket();
	wsa::error connect( _in const address &address );

private:
	SOCKET m_data;
	bool m_isconnected;
};

class request
{
public:
	static void make( _in cstr_t url );				// all-in-one

	static string_t prepare( _in cstr_t url );

private:
	static void get_host( _in cstr_t url, _out std::pair< cstr_t, cstr_t > &host );
};