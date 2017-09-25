// p_htmldownload.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <urlmon.h>

#include <fstream>

#pragma comment( lib, "urlmon.lib" )

namespace page
{
	void download(
		_in cstr_t url
	) {
		//assert( url );

		string_t url_full( L"http://" );
		url_full += url;
		url_full.push_back( L'/' );
		url = url_full.c_str();

		wprintf_s( L"%s( \"%s\" )...", __FUNCTIONW__, url );

		auto hr = URLDownloadToFile( nullptr, url, OFILE_NAME, 0, nullptr );		
		if ( S_OK != hr )
		{
			wprintf_s( L"URLDownloadToFile(): %08X\n", hr );
			throw hr;
		}

		wprintf_s( L" ok\n" );
	}

	unsigned get_size(
	) {
		
		wprintf_s( L"%s( \"%s\" )...", __FUNCTIONW__, OFILE_NAME );
		std::wifstream f( OFILE_NAME );
	
		if ( !f.is_open() )
			throw GetLastError();
	
		f.seekg( 0, std::ios_base::end );
		auto size = f.tellg();
		f.close();

		wprintf_s( L" ok, size: %u bytes\n", static_cast< unsigned >(size) );

		return size;
	}

}

int wmain(
	_in size_t arg_c, _in cstr_t arg_v[]
) {
    
	try {
		assert( 2 == arg_c );

		page::download( arg_v[1] );
		auto size = page::get_size();
	}
	catch (...) {
		wprintf_s( L"exception: -1\n" );
		return -1;
	}

	return 0;
}

