#ifdef _MSC_VER
// to avoid long name warnings
#pragma warning(disable:4786)
#endif

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <ios>

#include "OStreamMonitor.h"
#include "utils/compatibility.h"
#include "Param.h"
#include "Logger.h"

namespace core_library
{

    //using namespace std;

    Monitor& OStreamMonitor::operator()(void)
    {
	if (!out) {
	    std::string str = "OStreamMonitor: Could not write to the ooutput stream";
	    throw std::runtime_error(str);
	}

	if (firsttime) {

	    logger << debug << "First Generation" << std::endl;

	    for (iterator it = vec.begin (); it != vec.end (); ++it) {
		out << (*it)->longName ();
		out << delim << std::left << std::setfill(fill) << std::setw(width);
	    }
	    out << std::endl;

	    firsttime = false;
	} // if firstime

	// ok, now the real saving. write out
	//! @todo old verbose formatting, do we still need it?
	/*
	  for (iterator it = vec.begin (); it != vec.end (); ++it) {
	  // name: value
	  out << (*it)->longName () << ": " << (*it)->getValue () << std::endl;
	  } // for it in vec
	*/

	for (iterator it = vec.begin (); it != vec.end (); ++it) {
	    // value only
	    out << (*it)->getValue ();
	    out << delim << std::left << std::setfill(fill) << std::setw(width);
	} // for it in vec

	out << std::endl;
	logger << debug << "End of Generation" << std::endl;

	return *this;
    }

}
