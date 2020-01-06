

#pragma once


#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include "Exception.h"


#ifndef MAP_NOSYNC
#define MAP_NOSYNC 0
#endif


namespace JDWX {
namespace OS {


inline void fstat( int fd, struct stat * sb ) {
    if ( 0 == ::fstat( fd, sb ) )
        return;
    throw JDWX::Exception::Errno( "fstat" );
}


inline void ftruncate( int fd, off_t length ) {
    if ( 0 == ::ftruncate( fd, length ) )
        return;
    throw JDWX::Exception::Errno( "ftruncate" );
}


inline uid_t geteuid( void ) {
    return ::geteuid();
}


inline void * mmap( void * addr, size_t len, int prot, int flags, int fd,
                    off_t offset ) {
    void *pv = ::mmap( addr, len, prot, flags, fd, offset );
    if ( MAP_FAILED != pv )
        return pv;
    throw JDWX::Exception::Errno( "fstat" );
}


inline void nanosleep( struct timespec const * rqtp,
                       struct timespec * rmtp = nullptr ) {
    if ( -1 != ::nanosleep( rqtp, rmtp ) )
        return;
    throw JDWX::Exception::Errno( "nanosleep" );
}


inline void nanosleep( time_t tvSec, long tv_nsec ) {
    struct timespec ts = { tvSec, tv_nsec };
    JDWX::OS::nanosleep( & ts );
}


inline void setuid( uid_t uid ) {
    if ( 0 == ::setuid( uid ) )
        return;
    throw JDWX::Exception::Errno( "setuid" );
}


inline int shm_open( char const * path, int flags, mode_t mode ) {
    int shd = ::shm_open( path, flags, mode );
    if ( shd >= 0 )
        return shd;
    throw JDWX::Exception::Errno( "shm_open" );
}


inline int shm_open( std::string const & path, int flags, mode_t mode ) {
    return JDWX::OS::shm_open( path.c_str(), flags, mode );
}


inline void shm_unlink( char const * path ) {
    if ( 0 == ::shm_unlink( path ) )
        return;
    throw JDWX::Exception::Errno( "shm_unlink" );
}


inline size_t write( int d, const void *buf, size_t nbytes ) {
    ssize_t rc = ::write( d, buf, nbytes );
    if ( rc >= 0 )
        return static_cast< size_t >( rc );
    throw JDWX::Exception::Errno( "write" );
}


}  //  namespace OS
}  //  namespace JDWX


