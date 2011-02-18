// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

/*
(c) Thales group, 2010

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
    version 2 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Authors:
	Johann Dréo <johann.dreo@thalesgroup.com>
	Caner Candan <caner.candan@thalesgroup.com>
*/

#ifdef _INTERIX
#include <io.h>
#else // _INTERIX
#include <unistd.h>
#endif // ! _INTERIX

#include <fcntl.h>
#include <cstdlib>
#include <cstdio> // used to define EOF

#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <locale>

#include "Logger.h"

namespace core_library
{
    Logger	log;

    Logger::Logger()
	: std::ostream(&_obuf),
	  _selectedLevel(core_library::progress), _contextLevel(core_library::quiet),
	  _fd(2), _obuf(_fd, _contextLevel, _selectedLevel)
    {
	_standard_io_streams[&std::cout] = 1;
	_standard_io_streams[&std::clog] = 2;
	_standard_io_streams[&std::cerr] = 2;

	// /!\ If you want to add a level dont forget to add it at the header file in the enumerator Levels

	addLevel("quiet", core_library::quiet);
	addLevel("errors", core_library::errors);
	addLevel("warnings", core_library::warnings);
	addLevel("progress", core_library::progress);
	addLevel("logging", core_library::logging);
	addLevel("debug", core_library::debug);
	addLevel("xdebug", core_library::xdebug);
    }

    Logger::~Logger()
    {}

    std::string	Logger::className() const
    {
	return ("Logger");
    }

    void	Logger::addLevel(std::string name, core_library::Levels level)
    {
	_levels[name] = level;
	_sortedLevels.push_back(name);
    }

    void	Logger::printLevels() const
    {
	std::cout << "Available verbose levels:" << std::endl;

	for (std::vector<std::string>::const_iterator it = _sortedLevels.begin(), end = _sortedLevels.end();
	     it != end; ++it)
	    {
		std::cout << "\t" << *it << std::endl;
	    }

	::exit(0);
    }

    Logger&	operator<<(Logger& l, const core_library::Levels lvl)
    {
	l._contextLevel = lvl;
	return l;
    }

    Logger&	operator<<(Logger& l, core_library::file f)
    {
	l._fd = ::open(f._f.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0644);
	return l;
    }

    Logger&	operator<<(Logger& l, core_library::setlevel v)
    {
	l._selectedLevel = (v._lvl < 0 ? l._levels[v._v] : v._lvl);
	return l;
    }

    Logger&	operator<<(Logger& l, std::ostream& os)
    {
	if (l._standard_io_streams.find(&os) != l._standard_io_streams.end())
	    {
		l._fd = l._standard_io_streams[&os];
	    }
	return l;
    }

    Logger::outbuf::outbuf(const int& fd,
			     const core_library::Levels& contexlvl,
			     const core_library::Levels& selectedlvl)
	: _fd(fd), _contextLevel(contexlvl), _selectedLevel(selectedlvl)
    {}

    int	Logger::outbuf::overflow(int_type c)
    {
	if (_selectedLevel >= _contextLevel)
	    {
		if (_fd >= 0 && c != EOF)
		    {
			ssize_t	num;
			num = ::write(_fd, &c, 1);
		    }
	    }
	return c;
    }

    file::file(const std::string f)
	: _f(f)
    {}

    setlevel::setlevel(const std::string v)
	: _v(v), _lvl((Levels)-1)
    {}

    setlevel::setlevel(const Levels lvl)
	: _v(std::string("")), _lvl(lvl)
    {}
}
