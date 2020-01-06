

#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/types.h>


#include "OS.h"
#include "RingBuffer.h"


using std::cin;
using std::clog;
using std::cout;
using std::endl;
using std::exception;
using std::getline;
using std::runtime_error;
using std::string;


namespace OS = JDWX::OS;


int main( int argc, char const * argv[] ) {
	try {
		if ( argc < 2 )
			throw runtime_error( "Must specify log path on command line." );
		unsigned szBuffer = 1048576;
		if ( argc >= 3 )
			szBuffer = static_cast< unsigned >( std::stoul( argv[ 2 ] ) );
		if ( szBuffer < 1024 )
			throw runtime_error( "Buffer size must be at least 1024 bytes." );
		RingBuffer rb( argv[ 1 ], szBuffer );
		char strBuffer[ 1024 ];
		ssize_t sz;
		if ( 0 == OS::geteuid() )
			OS::setuid( 65534 );
		while ( true ) {
			sz = read( 0, strBuffer, 1024 );
			if ( sz <= 0 )
                break;
			rb.write( strBuffer, static_cast< unsigned>( sz ) );
		}
	} catch ( exception & r_ex ) {
		clog << argv[0] << " FATAL: " << r_ex.what() << endl;
		return 10;
	}
	return 0;
}


