/// \file   inbyte.h
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the declaration for converting between Byte, KByte, MByte and GByte.

#ifndef INBYTE_H
#define INBYTE_H

#include "intypes.h"

namespace iNumerics
{

	class KByte;
	class MByte;
	class GByte;

	/**
	 * 
	 * \author Michael Hoffer, 2012
	 * \brief Class for easily converting between Bytes, KBytes, MBytes and GBytes.
	 */
	class Byte
	{
		public:
			Byte();
			Byte ( inULong b );
			Byte ( KByte kb );
			Byte ( MByte mb );
			Byte ( GByte gb );

			Byte operator * ( const inULong value ) const;
			Byte operator * ( const Byte value ) const;
			Byte operator - ( const Byte value ) const;
			Byte operator -= ( const Byte value );
			Byte operator + ( const Byte value ) const;
			Byte operator += ( const Byte value );

			const unsigned long value() const
			{
				return _bytes;
			};

		private:
			inULong _bytes;

			friend class KByte;
			friend class MByte;
			friend class GByte;
	};

	/**
	 * \author Michael Hoffer, 2012
	 * \brief Class for easily converting between Bytes, KBytes, MBytes and GBytes.
	 */
	class KByte
	{
		public:
			KByte();
			KByte ( const inDouble b );
			KByte ( Byte b );
			KByte ( MByte mb );
			KByte ( GByte gb );

			KByte  operator * ( const inULong value ) const;
			KByte  operator * ( const inDouble value ) const;
			KByte  operator * ( const KByte value ) const;

			KByte operator - ( const KByte value ) const;
			KByte operator -= ( const KByte value );
			KByte operator + ( const KByte value ) const;
			KByte operator += ( const KByte value );

			const inDouble value() const
			{
				return _kbytes;
			};

		private:
			inDouble _kbytes;

			friend class Byte;
			friend class MByte;
			friend class GByte;
	};

	/**
	 * \author Michael Hoffer, 2012
	 * \brief Class for easily converting between Bytes, KBytes, MBytes and GBytes.
	 */
	class MByte
	{
		public:
			MByte();
			MByte ( const inDouble b );
			MByte ( Byte b );
			MByte ( KByte kb );
			MByte ( GByte gb );

			MByte  operator * ( const inULong value ) const;
			MByte  operator * ( const inDouble value ) const;
			MByte  operator * ( const MByte value ) const;

			MByte operator - ( const MByte value ) const;
			MByte operator -= ( const MByte value );
			MByte operator + ( const MByte value ) const;
			MByte operator += ( const MByte value );

			const inDouble value() const
			{
				return _mbytes;
			};

		private:
			inDouble _mbytes;

			friend class Byte;
			friend class KByte;
			friend class GByte;
	};

	/**
	 * \author Michael Hoffer, 2012
	 * \brief Class for easily converting between Bytes, KBytes, MBytes and GBytes.
	 */
	class GByte
	{
		public:
			GByte();
			GByte ( const inDouble b );
			GByte ( Byte b );
			GByte ( KByte kb );
			GByte ( MByte gb );

			GByte  operator * ( const inULong value ) const;
			GByte  operator * ( const inDouble value ) const;
			GByte  operator * ( const GByte value ) const;

			GByte operator - ( const GByte value ) const;
			GByte operator -= ( const GByte value );
			GByte operator + ( const GByte value ) const;
			GByte operator += ( const GByte value );

			const inDouble value() const
			{
				return _gbytes;
			};

		private:
			inDouble _gbytes;

			friend class Byte;
			friend class KByte;
			friend class MByte;
	};
}

#ifndef INBYTE_HPP
//#define INCLUDED_IN_INBYTE_H
#include "inbyte.hpp"
//#undef INCLUDED_IN_INBYTE_H
#endif /*INBYTE_HPP*/

#endif /*INBYTE_H*/

