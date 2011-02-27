// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// CheckPoint.h
// (c) Maarten Keijzer, Marc Schoenauer and GeNeura Team, 2000
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
             mkeijzer@dhi.dk
	     Caner Candan <caner@candan.fr>, http://caner.candan.fr
 */
//-----------------------------------------------------------------------------

#ifndef _core_library_CheckPoint_h
#define _core_library_CheckPoint_h

#include <vector>

#include "Continue.h"
#include "Updater.h"
#include "Monitor.h"
#include "Stat.h"

namespace core_library
{

    /** @defgroup Checkpoints Checkpointing
     *
     * Checkpoints are supposed to be called perodically (for instance at each generation) and
     * will call every functors you put in them.
     *
     * Use them with Stats, Updater and Monitor to get statistics at each generation.
     *
     * @see Stat
     * @see Monitor
     * @see Updater
     *
     * Example of a test program using checkpointing:
     * @include t-eoCheckpointing.cpp
     *
     * @ingroup Utilities
     *
     * @{
     */

    /** CheckPoint is a container class.
	It contains std::vectors of (pointers to) 
	Continue    (modif. MS July 16. 2002)
	Stats, Updater and Monitor
	it is an Continue, so its operator() will be called every generation - 
	and will return the contained-combined-Continue result
	but before that it will call in turn every single 
	{statistics, updaters, monitors} that it has been given,
	and after that, if stopping, all lastCall methods of the above.
    */
    template <class Atom>
    class CheckPoint : public Continue<Atom>
    {
    public :

	CheckPoint(Continue<Atom>& cont)
	{
	    _continuators.push_back(&cont);
	}

	bool operator()(const Atom& value)
	{
	    for ( unsigned int i = 0, size = _stats.size(); i < size; ++i )
		{
		    (*_stats[i])( value );
		}

	    for ( unsigned int i = 0, size = _updaters.size(); i < size; ++i )
		{
		    (*_updaters[i])();
		}

	    for ( unsigned int i = 0, size = _monitors.size(); i < size; ++i )
		{
		    (*_monitors[i])();
		}

	    bool bContinue = true;
	    for ( unsigned int i = 0, size = _continuators.size(); i < size; ++i )
		{
		    if ( !(*_continuators[i])( value ) )
			{
			    bContinue = false;
			}
		}

	    if ( !bContinue )
		{
		    for ( unsigned int i = 0, size = _stats.size(); i < size; ++i )
			{
			    _stats[i]->lastCall( value );
			}

		    for ( unsigned int i = 0, size = _updaters.size(); i < size; ++i )
			{
			    _updaters[i]->lastCall();
			}

		    for ( unsigned int i = 0, size = _monitors.size(); i < size; ++i )
			{
			    _monitors[i]->lastCall();
			}
		}

	    return bContinue;
	}

	void add(Continue<Atom>& cont) { _continuators.push_back(&cont); }
	void add(StatBase<Atom>& stat) { _stats.push_back(&stat); }
	void add(Monitor& mon)        { _monitors.push_back(&mon); }
	void add(Updater& upd)        { _updaters.push_back(&upd); }

	virtual std::string className(void) const { return "CheckPoint"; }

	std::string allClassNames() const
	{
	    std::string s("\n" + className() + "\n");

	    s += "Stats\n";
	    for ( unsigned int i = 0, size = _stats.size(); i < size; ++i )
		{
		    s += _stats[i]->className() + "\n";
		}
	    s += "\n";

	    s += "Updaters\n";
	    for ( unsigned int i = 0; i < _updaters.size(); ++i )
		{
		    s += _updaters[i]->className() + "\n";
		}
	    s += "\n";

	    s += "Monitors\n";
	    for ( unsigned int i = 0; i < _monitors.size(); ++i )
		{
		    s += _monitors[i]->className() + "\n";
		}
	    s += "\n";

	    s += "Continuators\n";
	    for ( unsigned int i = 0, size = _continuators.size(); i < size; ++i )
		{
		    s += _continuators[i]->className() + "\n";
		}
	    s += "\n";

	    return s;
	}

    private:
	std::vector<Continue<Atom>*> _continuators;
	std::vector<StatBase<Atom>*> _stats;
	std::vector<Monitor*> _monitors;
	std::vector<Updater*> _updaters;
    };

}

/** @} */
#endif // !_core_library_CheckPoint_h
