#ifndef FORMATOUTPUT_HPP
#define FORMATOUTPUT_HPP

#include "util/LazyArgumentEnum.hpp"
#include "util/StringedEnum.hpp"
#include <string>
#include <deque>
#include <memory>

class Node;

LAZY_ARGUMENT_ENUM(SolutionType, solutionTypes,
		(coordinate)(direction)(minimal))

STRINGED_ENUM(SolutionQuality, (none)(good)(bad))

std::string formatOutput(const std::string& format,
		const std::deque<std::shared_ptr<Node>>& solution,
		SolutionQuality solutionQuality);


#endif /* FORMATOUTPUT_HPP */
