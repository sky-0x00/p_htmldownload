#include "stdafx.h"
#include "u_network.h"
#include "Ws2tcpip.h"

#pragma comment( lib,"ws2_32.lib" )

#define CHAR_HTTP_SLASH		L'/'

//------------------------------------------------------------------------------------------------------
/*static*/ void wsa::init(
	_out WSADATA &data
) {
	const auto result = WSAStartup( MAKEWORD(2, 2), &data );
	if ( S_OK != result )
		throw result;
}

/*static*/ void wsa::cleanup(
) {
	const auto result = WSACleanup();
	if ( S_OK != result )
		throw get_error();
}

/*static*/ wsa::error wsa::get_error(
) {
	return WSAGetLastError();
}

//------------------------------------------------------------------------------------------------------
wsa::wsa(
) {
	wsa::init( m_data );
}

wsa::~wsa(
) {
	wsa::cleanup();
}

//------------------------------------------------------------------------------------------------------
/*static*/ void request::make( 
	_in cstr_t url 
) {
	prepare( url );

}

/*static*/ string_t request::prepare( 
	_in cstr_t url 
) {
	assert( url );

	std::pair< cstr_t, cstr_t > host;
	get_host( url, host );

	string_t s_request( L"GET / HTTP/1.1\nHost: " );
	s_request += string_t( host.first, host.second - host.first );
	s_request += L"\nConnection: close\n\n";

	return s_request;
}

//------------------------------------------------------------------------------------------------------
/*static*/ void request::get_host( 
	_in cstr_t url, 
	_out std::pair< cstr_t, cstr_t > &host
) {
	//assert( url );
	
	host.first = url;
	auto is_slash = false;

	cstr_t p = url;
	for ( ; *p; ++p )
	{
		if ( p[0] == CHAR_HTTP_SLASH )
		{
			if ( is_slash )
				continue;

			is_slash = true;
			
			if ( p[1] != CHAR_HTTP_SLASH )
				break;
		}
		else 
		{
			if ( is_slash )
			{
				host.first = p;
				is_slash = false;
			}
		}
	}
	host.second = p;
}

//------------------------------------------------------------------------------------------------------
socket::socket(
) :
	m_isconnected( false )
{
	m_data = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( INVALID_SOCKET == m_data )
		throw wsa::get_error();
}

socket::~socket(
) {
	/*const auto result =*/ closesocket( m_data );
	//if ( SOCKET_ERROR == result )
	//	throw wsa::get_error();
}

wsa::error socket::connect( 
	_in const address &address 
) {
	assert( !m_isconnected );

	const auto result = ::connect( m_data, &static_cast< const sockaddr& >( address ), sizeof( sockaddr ) );	
	switch (result)
	{
		case S_OK:
			m_isconnected = true;
			return S_OK;
		case SOCKET_ERROR:
			return wsa::get_error();
		default:
			throw wsa::get_error();
	}
}

//------------------------------------------------------------------------------------------------------
socket::address::address(
	_in host_t host, 
	_in port_t port /*= PORT_HTTP*/
) :
	m_data( {0} )
{
	m_data.sin_family = AF_INET;
	m_data.sin_port = htons( port );
	m_data.sin_addr.s_addr = host;						// неявно к типу ULONG
	
	//m_data.sin_addr.S_un.S_un_b.s_b1 = host[0];		// верный порядок
	//m_data.sin_addr.S_un.S_un_b.s_b2 = host[1];
	//m_data.sin_addr.S_un.S_un_b.s_b3 = host[2];
	//m_data.sin_addr.S_un.S_un_b.s_b4 = host[3];

	//m_data.sin_addr.S_un.S_un_b.s_b1 = host[3];
	//m_data.sin_addr.S_un.S_un_b.s_b2 = host[2];
	//m_data.sin_addr.S_un.S_un_b.s_b3 = host[1];
	//m_data.sin_addr.S_un.S_un_b.s_b4 = host[0];
}

socket::address::address(
	_in cstr_t host,
	_in port_t port /*= PORT_HTTP*/
) :
	m_data( {0} )
{
	assert( host );

	m_data.sin_family = AF_INET;
	m_data.sin_port = htons( port );
	
	ADDRINFOW *addr_info = nullptr;
	auto result = GetAddrInfo( host, nullptr, nullptr, &addr_info );
	if ( result != ERROR_SUCCESS )
		throw result;

	// берем первый элемент связного списка
	assert( addr_info[0].ai_addrlen == sizeof( m_data ) );		// 0x10
	assert( addr_info[0].ai_family == AF_INET );				// 0x2

	const SOCKADDR_IN *in_addr = reinterpret_cast< const SOCKADDR_IN* >( addr_info[0].ai_addr );
	m_data.sin_addr = in_addr->sin_addr;
}

socket::address::operator const sockaddr_in&(
) const {
	return m_data;
}
socket::address::operator const sockaddr&(
) const {
	return *reinterpret_cast< const sockaddr* >( &m_data );
}


//------------------------------------------------------------------------------------------------------
socket::address::ipv4::ipv4( 
	_in const ipv4 &ipv4 
) :
	m_data( ipv4.m_data ),
	m_string( ipv4.m_string )
{
}
socket::address::ipv4::ipv4( 
	_in ipv4 &&ipv4 
) {
	std::swap( *this, ipv4 );
}

socket::address::ipv4::ipv4( 
	_in const std::array< byte_t, 4 > &data 
) :
	m_data( data )
{
	//std::copy( data.begin(), data.end(), m_data.begin() );
}
socket::address::ipv4::ipv4( 
	_in std::array< byte_t, 4 > &&data 
) {
	m_data.swap( data );
}

const string_t& socket::address::ipv4::to_string(
) {
	if ( m_string.empty() )
	{
		m_string.reserve(16);		
		char_t buffer[4];

		auto error = _ultow_s( m_data[0], buffer, 10 );
		assert( !error );
		m_string += buffer;

		for ( auto i = 1; i < 4; ++i )
		{
			m_string.push_back( L'.' );
			error = _ultow_s( m_data[i], buffer, 10 );
			assert( !error );
			m_string += buffer;
		}
	}
	return m_string;
}

socket::address::ipv4::operator ULONG(
) const {
	return *reinterpret_cast< const ULONG* >( m_data.data() );
}

//------------------------------------------------------------------------------------------------------