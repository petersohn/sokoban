#ifndef FORMATOUTPUT_HPP
#define FORMATOUTPUT_HPP

#include "util/LazyArgumentEnum.hpp"
#include <string>
#include <deque>
#include <memory>

class Node;

LAZY_ARGUMENT_ENUM(SolutionType, solutionTypes, 
		(coordinate)(direction)(minimal))

std::string formatOutput(const std::string& format,
		const std::deque<std::shared_ptr<Node>>& solution);


#endif /* FORMATOUTPUT_HPP */
