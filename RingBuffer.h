

#pragma once


#include <string>


struct RingBuffer_Header {
	unsigned  uStart;
	unsigned  uEnd;
};


class RingBuffer {
public:
	         RingBuffer( std::string const & cr_stPath );
	         RingBuffer( std::string const & cr_stPath, unsigned i_szBuffer );
	unsigned avail( void ) const;
	void     dump( int i_fd ) const;
    [[ noreturn ]] 
	void     dumpForever( int i_fd, bool i_bZero ) const;
	void     dumpRange( int i_fd, unsigned i_uStart, unsigned i_uEnd ) const;
	unsigned maxsize( void ) const;
	void     shrink( unsigned );
	unsigned size( void ) const;
	void     write( char const * i_strInput, unsigned i_szInput );

protected:
	void     wait( unsigned i_uEnd ) const;

private:
	RingBuffer( void );

protected:
	bool                _bRollover;
	unsigned            _szBuffer;
	char              * _buffer;
	int                 _shd;
	RingBuffer_Header * _hdr;

};


/*
**  INLINE METHODS
*/


inline unsigned RingBuffer::avail() const {
    return _szBuffer - size();
}


inline unsigned RingBuffer::maxsize() const {
    return _szBuffer;
}


inline unsigned RingBuffer::size() const {
    return (_hdr->uStart > _hdr->uEnd) ?
        (_szBuffer + _hdr->uEnd - _hdr->uStart) : _hdr->uEnd - _hdr->uStart;
}



