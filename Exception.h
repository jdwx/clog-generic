

#pragma once


#include <cerrno>
#include <stdexcept>


namespace JDWX {
namespace Exception {


class Errno : public std::runtime_error {
public:

    Errno( char const * i_strFunction );
    Errno( std::string const & i_stFuncton );
    Errno( int i_errno, std::string const & i_stFuncton );

    bool operator==( int i_err ) const;
    bool operator!=( int i_err ) const;

private:

    int _errno;

};


inline Errno::Errno( char const * i_strFunction )
        : Errno( std::string( i_strFunction ) ) {
}


inline Errno::Errno( std::string const & i_stFunction )
        : Errno( errno, i_stFunction ) {
}


inline Errno::Errno( int i_errno, std::string const & i_stFunction )
        : std::runtime_error(
            i_stFunction + " failed: (" + std::to_string( i_errno )
            + strerror( errno )
        ) {
    _errno = i_errno;
}


inline bool Errno::operator==( int i_errno ) const {
    return i_errno == _errno;
}


inline bool Errno::operator!=( int i_errno ) const {
    return i_errno != _errno;
}


}  //  namespace Exception
}  //  namespace JDWX


