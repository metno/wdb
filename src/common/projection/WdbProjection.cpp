/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301, USA
*/


/**
 * @addtogroup common
 * @{
 * @addtogroup projection
 * @{
 */
/**
 * @file
 * Implementation of wdbProjection routines
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include "WdbProjection.h"
// PROJECT INCLUDES
#include <wdbLogHandler.h>
// SYSTEM INCLUDES
#include <sstream>
#include <proj_api.h>
#include <boost/noncopyable.hpp>


using namespace std;
using namespace wdb;


WdbProjection::WdbProjection(const string & def)
	: projText_( def )
{
	projDef_ = pj_init_plus( projText_.c_str() );
	if ( !projDef_ )
		throw std::runtime_error( "Invalid PROJ definition: " + def );
}

WdbProjection::~WdbProjection()
{
	pj_free( projDef_ );
}

/// do the actual transformation
void
WdbProjection::transform(const WdbProjection & dest, size_t size, double * lon, double * lat) const
{
	//std::vector<double> alt( size );

	int error = pj_transform( projDef_, dest.projDef_, size, 0, lon, lat, NULL);
	if ( error )
	{
		ostringstream msg;
		msg << "Error during reprojection: " << pj_strerrno(error) << "." << '\n'
			<< "From:\t" << projText_ << '\n'
			<< "To:\t"<< dest.projText_ ;
		if ( size > 0 ) {
			msg << '\n' << "Coordinates: " << lon[0] << ' ' << lat[0];
		}

		throw std::runtime_error( msg.str() );
	}
}

/// do the actual transformation (datums only)
void
WdbProjection::datumTransform(const WdbProjection & dest, size_t size, double * lon, double * lat) const
{
	//std::vector<double> alt( size );

	int error = pj_datum_transform( projDef_, dest.projDef_, size, 0, lon, lat, NULL);
	if ( error )
	{
		ostringstream msg;
		msg << "Error during datum transform: "<< pj_strerrno(error) << "." << '\n'<< "From:\t"
				<< projText_ << '\n'<< "To:\t"<< dest.projText_;

		throw std::runtime_error( msg.str() );
	}
}

// Transform to default
void
WdbProjection::transformToDefault(size_t size, double * lon, double * lat) const
{
	WdbProjection dest( DEFAULT_PROJECTION );
	transform( dest, size, lon, lat );
}

// Transform from default
void
WdbProjection::transformFromDefault(size_t size, double * lon, double * lat) const
{
	WdbProjection dest( DEFAULT_PROJECTION );
	dest.transform( *this, size, lon, lat );
}


const std::string &
WdbProjection::str() const
{
	return projText_;
}



namespace {

/**
 * Handles automatic initalization and deinitialization of a projPJ.
 *
 * The underlying projPJ object can be obtained by using operator * (), eg.
 * ProjPjHandler h( ... ); whatever( * h );
 */
class ProjPjHandler : boost::noncopyable
{
	projPJ pj_;

public:
	/**
	 * Default Constructor
	 * @param	init	Proj String
	 */
	ProjPjHandler( const std::string & init )
		: pj_( pj_init_plus( init.c_str() ) )
	{
		/*
		// Doing this may be better, but it creates too much disturbances
		// in the method that use this class.
	    if ( ! pj )
	    {
	        const string message = "Could not create projection. Definition=" + init;
	        log.warn( message );
	        throw std::logic_error( message );
	    }
	    */
	}

	/// Destructor
	~ProjPjHandler()
	{
		if ( pj_ )
			pj_free( pj_ );
	}

	projPJ & operator * () { return pj_; }
	const projPJ & operator * () const { return pj_; }
};

} // namespace <Support Function>


/**
 * @}
 *
 * @}
 */
