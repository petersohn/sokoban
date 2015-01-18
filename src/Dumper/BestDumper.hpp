#ifndef SRC_DUMPER_BESTDUMPER_HPP
#define SRC_DUMPER_BESTDUMPER_HPP

#include "Dumper/Dumper.hpp"
#include <fstream>

class Table;

class BestDumper: public Dumper {
public:
	BestDumper(const std::string& filename):
		file(filename, std::ios::out | std::ios::trunc)
	{}

	virtual void initialStatus(const Status& status);
	virtual void addNode(const std::shared_ptr<Node>& node);
	virtual void addToSolution(const std::shared_ptr<Node>& node);
	virtual void expand(const std::shared_ptr<Node>& node);
	virtual void startPushing(const std::shared_ptr<Node>& node);
	virtual void push(const std::shared_ptr<Node>& node);
	virtual void reject(const std::shared_ptr<Node>& node, const char *text);
	virtual void save();
private:
	std::ofstream file;
	const Table* table = nullptr;
	float bestHeur = -1.0;
	std::size_t nodeNumber = 0;
};

#endif /* SRC_DUMPER_BESTDUMPER_HPP */

