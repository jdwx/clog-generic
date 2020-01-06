

/*
**  INCLUDES
*/


#include <exception>
#include <iostream>
#include <stdexcept>


#include "OS.h"
#include "RingBuffer.h"


/*
**  NAMESPACES
*/


using std::clog;
using std::cout;
using std::endl;
using std::exception;
using std::runtime_error;

namespace OS = JDWX::OS;


/*
**  PROTOTYPES
*/


void Usage(const char *argv0);


/*
**  FUNCTION MAIN
*/


int main(int argc, char *argv[]) {
	try {
		if (argc < 2 || argc > 3) {
			Usage(argv[0]);	
			return 1;
		}

		if (argc == 2) {
			RingBuffer(argv[1]).dump(1);
			return 0;
		}

		if (strcmp(argv[1], "-r") == 0) {
			OS::shm_unlink(argv[2]);
			return 0;
		}

		if ( 0 == strcmp( argv[ 1 ], "-f" ) )
			RingBuffer( argv[ 2 ] ).dumpForever( 1, false );

		if ( 0 == strcmp( argv[1], "-z" ) )
			RingBuffer( argv[ 2 ] ).dumpForever( 1, true );

		Usage( argv[ 0 ] );
		return 2;

	} catch ( exception & r_ex ) {
		clog << argv[0] << " FATAL: " << r_ex.what() << endl; 
		return 10;
	}

}


/*
**  FUNCTION USAGE
*/


void Usage(const char *argv0) {
	clog << endl << "Usage:" << endl;
	clog << "$ " << argv0 << " [-f|-r|-z] <logname>" << endl << endl;
	clog << "-f = wait for additional data to be appended to the log" << endl;
	clog << "-r = remove <logname>" << endl;
	clog << "-z = like -f but pretend the buffer starts empty" << endl;
	clog << endl;
}


