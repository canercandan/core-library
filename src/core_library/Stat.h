// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// Stat.h
// (c) Marc Schoenauer, Maarten Keijzer and GeNeura Team, 2000
// (c) 2010 Thales group
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

Contact: http://eodev.sourceforge.net

    Authors:
        todos@geneura.ugr.es, http://geneura.ugr.es
        Marc.Schoenauer@polytechnique.fr
        mkeijzer@dhi.dk
        Johann Dréo <johann.dreo@thalesgroup.com>
 */
//-----------------------------------------------------------------------------

#ifndef _core_library_Stat_h
#define _core_library_Stat_h

#include <numeric> // accumulate

#include "UF.h"
#include "Param.h"
#include "Monitor.h"

namespace core_library
{

    /** @defgroup Stats Statistics computation
     *
     * Compute various statistics on a valueulation.
     *
     * Objects of those classes are generally called by an CheckPoint 
     * to compute statistics about the valueulation at a given generation.
     * As they inherit from ValueParam, they can be printed drectly,
     * for instance by an Monitor.
     *
     * @see CheckPoint
     * @see Monitor
     *
     * @ingroup Utilities
     * @{
     */

    /**
       Base class for all statistics that need to be calculated
       over the (unsorted) valueulation
       (I guess it is not really necessary? MS.
       Depstd::ends, there might be reasons to have a stat that is not an ValueParam,
       but maybe I'm just kidding myself, MK)
    */
    template <class Atom>
    class StatBase : public UF<const Atom&, void>
    {
    public:
	virtual void lastCall(const Atom&) {}
	virtual std::string className(void) const { return "StatBase"; }
    };


    template <class Atom> class CheckPoint;

    /**
       The actual class that will be used as base for all statistics
       that need to be calculated over the (unsorted) valueulation
       It is an StatBase AND an ValueParam so it can be used in Monitors.
    */
    template <class Atom, class T>
    class Stat : public ValueParam<T>, public StatBase<Atom>
    {
    public:

	Stat(T _value, std::string _description)
	    : ValueParam<T>(_value, _description)
        {}

	virtual std::string className(void) const
        { return "Stat"; }


	Stat<Atom, T>& addTo(CheckPoint<Atom>& cp)        { cp.add(*this);  return *this; }
	Stat<Atom, T>& addTo(Monitor& mon)               { mon.add(*this); return *this; }
    };

}

/** @} */
#endif // !_core_library_Stat_h
