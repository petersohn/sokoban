#include "Dumper/TextDumper.hpp"
#include "Status/Status.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Status/Table.hpp"
#include <string>

class FilterVisitor {
	const std::string& text;
public:
	using result_type = bool;

	FilterVisitor(const std::string& text):text(text) {}

	result_type operator()(TextDumper::NoFilter) const
	{
		return true;
	}
	result_type operator()(const std::string& filter) const
	{
		return text.find(filter) != std::string::npos;
	}
	result_type operator()(const std::regex& filter) const
	{
		return std::regex_search(text, filter);
	}
};

void TextDumper::dump(const Node& node, const std::string& text)
{
	if (boost::apply_visitor(FilterVisitor{text}, filter_)) {
		dumpNode(file_, *table_, node, text);
	}
}

void TextDumper::initialStatus(const Status &status) {
	std::cerr << "Initial status" << std::endl;
	table_ = &status.table();
	dumpStatus(file_, status, "Initial status");
}

void TextDumper::addNode(const std::shared_ptr<Node>& node) {
	dump(*node, "Added");
}

void TextDumper::addToSolution(const std::shared_ptr<Node>& /*node*/) {
// nothing to be done here
}

void TextDumper::expand(const std::shared_ptr<Node>& node) {
	dump(*node, "\nExpanded");
}

void TextDumper::startPushing(const std::shared_ptr<Node>& node) {
	dump(*node, "Pushing");
}

void TextDumper::push(const std::shared_ptr<Node>& node) {
	dump(*node, "Pushed out");
}

void TextDumper::reject(const std::shared_ptr<Node>& node, const char *text) {
	dump(*node, "Rejected: " + std::string(text));
}

void TextDumper::save() {
	file_.flush();
}
