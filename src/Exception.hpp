#ifndef SRC_EXCEPTION_HPP
#define SRC_EXCEPTION_HPP

#include <exception>

class SokobanException:public std::exception
{
	virtual const char* what() const throw()
	{
		return "Sokoban exception";
	}
};


#endif /* SRC_EXCEPTION_HPP */
