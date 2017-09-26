#pragma once

#include "stdafx.h"
#include <winsock2.h>
#include <array>

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
		class ipv4
		{
		public:
			ipv4( _in const ipv4 &ipv4 );
			ipv4( _in ipv4 &&ipv4 );
			ipv4( _in const std::array< byte_t, 4 > &data );
			ipv4( _in std::array< byte_t, 4 > &&data );
			const string_t& to_string();
			operator ULONG() const;
		private:
			std::array< byte_t, 4 > m_data;
			string_t m_string;
		};

		typedef ipv4 host_t;
		typedef unsigned short port_t;
		
		address( _in host_t host, _in port_t port = PORT_HTTP );		// host - by address
		address( _in cstr_t host, _in port_t port = PORT_HTTP );		// host - by dnsname

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