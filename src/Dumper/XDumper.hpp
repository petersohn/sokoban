#ifndef XDUMPER_H_
#define XDUMPER_H_

#include "Dumper/XML.hpp"
#include "Dumper/Dumper.hpp"
#include <memory>
#include <map>
#include <memory>

class Table;

class XDumper: public Dumper {
    XDumper(const XDumper &);
    const Table* table_;
    typedef std::map<std::shared_ptr<Node>, std::shared_ptr<xml::XMLElement> > MapType;
    MapType elements_;
    std::string filename_;

    typedef std::shared_ptr<xml::XMLElement> ElementPtr;

    ElementPtr createDumpElement(const std::string& s);
    ElementPtr getElement(const std::shared_ptr<Node>& node);
    ElementPtr doAddNode(const std::shared_ptr<Node>& node);
public:
    XDumper(const std::string& filename);
    void initialStatus(const Status& status) override;
    void addNode(const std::shared_ptr<Node>& node) override;
    void addToSolution(const std::shared_ptr<Node>& node) override;
    void expand(const std::shared_ptr<Node>& node) override;
    void startPushing(const std::shared_ptr<Node>& node) override;
    void push(const std::shared_ptr<Node>& node) override;
    void reject(const std::shared_ptr<Node>& node, const char *text) override;
    void clear();
    void save() override;
    void dump() const;
};

#endif /* XDUMPER_H_ */
