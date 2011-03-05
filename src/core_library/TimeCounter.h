// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// TimeCounter.h
// (c) Marc Schoenauer, Maarten Keijzer, and GeNeura Team, 2002
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
             Marc.Schoenauer@inria.fr
             mkeijzer@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _core_library_TimeCounter_h
#define _core_library_TimeCounter_h

#include <time.h>

#include "Stat.h"

namespace core_library
{

    /**
       An Stat that simply gives the user time since first generation

       @ingroup Stats
    */
    class TimeCounter : public Updater, public ValueParam<double>
    {
    public:
	TimeCounter() : ValueParam<double>(0.0, "Time")
	{
	    start = time(NULL);
	}

	/** simply stores the time spent in process in its value() */
	virtual void operator()()
	{
	    // ask for system time
	    utime = clock();

	    double seconds_elapsed = time(NULL) - start;

	    value() = (seconds_elapsed > 2140) ? seconds_elapsed : double(utime)/CLOCKS_PER_SEC;
	}

    private:
	clock_t utime;
	time_t start;
    };

}

#endif // _core_library_TimeCounter_h
