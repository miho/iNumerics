/// \file   inbyte.cpp
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the definition for converting between Bytes, KBytes, MBytes and GBytes.

#include "inbyte.h"
#include <cmath>

namespace iNumerics
{
	Byte::Byte()
	{
		_bytes = 0;
	}

	Byte::Byte ( inULong b )
	{
		_bytes = b;
	}

	Byte::Byte ( KByte kb )
	{
		_bytes = inULong ( kb._kbytes ) * 1024;
	}

	Byte::Byte ( MByte mb )
	{
		_bytes = inULong ( mb._mbytes * 1024 * 1024 );
	}

	Byte::Byte ( GByte gb )
	{
		_bytes = inULong ( gb._gbytes * 1024 * 1024 * 1024 );
	}

	Byte Byte::operator * ( const inULong value ) const
	{
		Byte b ( 0 );
		b._bytes = _bytes * value;
		return b;
	}

	Byte Byte::operator * ( const Byte value ) const
	{
		Byte b ( 0 );
		inULong v_bytes = value._bytes;
		b = ( *this ) * v_bytes;
		return b;
	}

	/*Byte Byte::operator- ( const Byte value ) const
	{
		Byte b ( 0 );
		b._bytes = abs(_bytes - value._bytes);
		return b;
	}

	Byte Byte::operator-= ( const Byte value )
	{
		_bytes = abs(_bytes - value._bytes);
		return *this;
	}*/

	Byte Byte::operator+ ( const Byte value ) const
	{
		Byte b ( 0 );
		b._bytes = _bytes + value._bytes;
		return b;
	}

	Byte Byte::operator+= ( const Byte value )
	{
		_bytes += value._bytes;
		return *this;
	}

	KByte::KByte()
	{
		_kbytes = 0;
	}

	KByte::KByte ( Byte b )
	{
		_kbytes = b._bytes / 1024.0f;
	}

	KByte::KByte ( const inDouble b )
	{
		_kbytes = b;
	}

	KByte::KByte ( MByte mb )
	{
		_kbytes = mb._mbytes * 1024;
	}

	KByte::KByte ( GByte gb )
	{
		_kbytes = gb._gbytes * 1024 * 1024;
	}


	KByte KByte::operator * ( const inULong value ) const
	{
		KByte b ( 0 );
		b._kbytes = std::abs(_kbytes * value);
		return b;
	}

	KByte KByte::operator * ( const inDouble value ) const
	{
		KByte b ( 0 );
		b._kbytes = _kbytes *value;
		return b;
	}

	KByte KByte::operator * ( const KByte value ) const
	{
		KByte b ( 0 );
		b = ( *this ) * KByte ( value );
		return b;
	}


	MByte::MByte()
	{
		_mbytes = 0;
	}


	MByte::MByte ( Byte b )
	{
		_mbytes = b._bytes / 1024.0f / 1024.0f;
	}

	MByte::MByte ( KByte kb )
	{
		_mbytes = kb._kbytes / 1024.0f;
	}

	MByte::MByte ( const inDouble mb )
	{
		_mbytes = mb;
	}

	MByte::MByte ( GByte gb )
	{
		_mbytes = gb._gbytes * 1024;
	}

	MByte MByte::operator * ( const inULong value ) const
	{
		MByte b ( 0 );
		b._mbytes = _mbytes * value;
		return b;
	}

	MByte MByte::operator * ( const inDouble value ) const
	{
		MByte b ( 0 );
		b._mbytes = _mbytes * value;
		return b;
	}

	MByte MByte::operator * ( const MByte value ) const
	{
		MByte b ( 0 );
		inDouble v_mbytes = value._mbytes;
		b = ( *this ) * v_mbytes;
		return b;
	}

	/*MByte MByte::operator- ( const MByte value ) const
	{
		MByte b ( 0 );
		b._mbytes = abs(_mbytes - value._mbytes);
		return b;
	}

	MByte MByte::operator-= ( const MByte value )
	{
		_mbytes = abs(_mbytes - value._mbytes);
		return *this;
	}*/

	MByte MByte::operator+ ( const MByte value ) const
	{
		MByte b ( 0 );
		b._mbytes = _mbytes + value._mbytes;
		return b;
	}

	MByte MByte::operator+= ( const MByte value )
	{
		_mbytes += value._mbytes;
		return *this;
	}


	GByte::GByte()
	{
		_gbytes = 0;
	}

	GByte::GByte ( Byte b )
	{
		_gbytes = b._bytes / 1024.0f / 1024.0f / 1024.0f;
	}

	GByte::GByte ( KByte kb )
	{
		_gbytes = kb._kbytes / 1024.0f / 1024.0f;
	}

	GByte::GByte ( MByte kb )
	{
		_gbytes = kb._mbytes / 1024.0f;
	}

	GByte::GByte ( const inDouble gb )
	{
		_gbytes = gb;
	}

	GByte GByte::operator * ( const inULong value ) const
	{
		GByte b ( 0 );
		b._gbytes = _gbytes * value;
		return b;
	}

	GByte GByte::operator * ( const inDouble value ) const
	{
		GByte b ( 0 );
		b._gbytes = _gbytes * value;
		return b;
	}

	GByte GByte::operator * ( const GByte value ) const
	{
		GByte b ( 0 );
		inDouble v_gbytes = value._gbytes;
		b = ( *this ) * v_gbytes;
		return b;
	}

	/*GByte GByte::operator- ( const GByte value ) const
	{
		GByte b ( 0 );
		b._gbytes = abs(_gbytes - value._gbytes);
		return b;
	}

	GByte GByte::operator-= ( const GByte value )
	{
		_gbytes = abs(_gbytes - value._gbytes);
		return *this;
	}*/

	GByte GByte::operator+ ( const GByte value ) const
	{
		GByte b ( 0 );
		b._gbytes = _gbytes + value._gbytes;
		return b;
	}

	GByte GByte::operator+= ( const GByte value )
	{
		_gbytes += value._gbytes;
		return *this;
	}

}
