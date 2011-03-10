// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// TolContinue.h
// (c) GeNeura Team, 1999, Marc Schoenauer 2001
/* 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: todos@geneura.ugr.es, http://geneura.ugr.es
             Marc.Schoenauer@polytechnique.fr
	     Caner Candan <caner@candan.fr>, http://caner.candan.fr
 */
//-----------------------------------------------------------------------------

#ifndef _core_library_TolContinue_h
#define _core_library_TolContinue_h

#include "Continue.h"

namespace core_library
{

    /**
     * Continues until a tolerance value has been reached
     *
     * @ingroup Continuators
     */
    template< class Atom >
    class TolContinue : public Continue< Atom >
    {
    public:
	/// Ctor
	TolContinue( Atom tol ) : _tol( tol ) {}

	/** Returns false when the tolerance has been done
	 */
	virtual bool operator() ( const Atom& value )
	{
	    if ( value <= _tol )
		{
		    logger << progress << "STOP in TolContinue: Reached the tolerance's value [" << _tol << "]" << std::endl;
		    return false;
		}
	    return true;
	}

	virtual std::string className(void) const { return "TolContinue"; }
    private:
	Atom _tol;
    };

}

#endif // !_core_library_TolContinue_h
