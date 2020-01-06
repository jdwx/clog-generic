

#include "RingBuffer.h"


#include <iostream>
#include <stdexcept>


#include "OS.h"


using std::cerr;
using std::clog;
using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::to_string;


namespace OS = JDWX::OS;


RingBuffer::RingBuffer() {
	_bRollover = false;
}


RingBuffer::RingBuffer( std::string const & cr_stPath ) : RingBuffer() {

	struct stat sb;
    unsigned    szFile;

	_shd = OS::shm_open( cr_stPath.c_str(), O_RDONLY, 0644 );
	OS::fstat( _shd, & sb );

    szFile = static_cast< unsigned >( sb.st_size );
	_szBuffer = szFile - sizeof( RingBuffer_Header );
	_hdr = reinterpret_cast< RingBuffer_Header * >(
        OS::mmap( nullptr, szFile, PROT_READ,
                  MAP_SHARED|MAP_NOSYNC, _shd, 0 )
    );
	_buffer = reinterpret_cast< char* >( _hdr + 1 );
}


RingBuffer::RingBuffer( std::string const & cr_stPath,
                        unsigned i_szBuffer ) : RingBuffer() {

	bool     bInit;
	unsigned sz;

	_szBuffer = i_szBuffer;
    sz = sizeof( RingBuffer_Header ) + _szBuffer;
    bInit = false;
	try {
		_shd = OS::shm_open( cr_stPath, O_RDWR, 0644 );
		struct stat sb;
		OS::fstat( _shd, & sb );
		if ( sb.st_size != sz ) {
			string st( "size mismatch " );
			st += to_string( sb.st_size );
			st += " != ";
			st += to_string( sz );
			throw runtime_error( st );
		}
	} catch ( JDWX::Exception::Errno & r_ex) {
		if ( r_ex != ENOENT )
			throw r_ex;
		_shd = OS::shm_open(cr_stPath.c_str(), O_RDWR|O_CREAT, 0644);
		OS::ftruncate(_shd, sz);
		bInit = true;
	}
	_hdr = reinterpret_cast< RingBuffer_Header * >(
        OS::mmap( nullptr, sz, PROT_READ|PROT_WRITE,
                  MAP_SHARED|MAP_NOSYNC, _shd, 0 )
    );
	_buffer = reinterpret_cast< char * >(_hdr + 1);
	if ( bInit ) {
		_hdr->uStart = 0;
		_hdr->uEnd = 0;
	}
}


void RingBuffer::dump( int i_fd ) const {
	dumpRange( i_fd, _hdr->uStart, _hdr->uEnd );
}


[[ noreturn ]]
void RingBuffer::dumpForever( int i_fd, bool i_bZero ) const {

	unsigned uStart = i_bZero ? _hdr->uEnd : _hdr->uStart;
	unsigned uEnd;

	while ( true ) {
		uEnd = _hdr->uEnd;
		dumpRange( i_fd, uStart, uEnd );
		uStart = uEnd;
		if ( uEnd != _hdr->uEnd )
			continue;
		wait( uEnd );
	}
}


void RingBuffer::dumpRange( int i_fd, unsigned i_uStart,
                            unsigned i_uEnd ) const {

	if ( i_uStart == i_uEnd )
        return;

	if ( i_uStart > i_uEnd ) {
		OS::write( i_fd, _buffer + i_uStart, _szBuffer - i_uStart );
		i_uStart = 0;
	}

	OS::write( i_fd, _buffer + i_uStart, i_uEnd - i_uStart );

}


void RingBuffer::wait( unsigned i_uEnd ) const {

	unsigned lNano = 1953125;

	while ( true ) {
		if (_hdr->uEnd != i_uEnd)
            return;
		OS::nanosleep( 0, lNano );
		 if ( lNano < 250000000 )
			lNano *= 2;
	}

}


void RingBuffer::write( char const * i_strInput, unsigned i_szInput ) {
	unsigned & uEnd   = _hdr->uEnd;
	unsigned & uStart = _hdr->uStart;
	while ( i_szInput > 0 ) {
		unsigned szWrite = _szBuffer - uEnd;
		if ( i_szInput < szWrite )
			szWrite = i_szInput;
		unsigned uNewEnd = ( uEnd + szWrite ) % _szBuffer;
		if ( uNewEnd <= uEnd )
			_bRollover = true;
		if ( _bRollover )
			uStart = ( uNewEnd + 1 ) % _szBuffer;
		memcpy( _buffer + uEnd, i_strInput, szWrite );
		// cout << "Wrote " << szWrite << " bytes at " << uEnd << "." << endl;
		uEnd = uNewEnd;
		i_szInput -= szWrite;
		i_strInput += szWrite;
		// cout << "Buffer runs from " << uStart << " to " << uEnd << "." << endl;
	}
}


