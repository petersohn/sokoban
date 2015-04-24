#include "Status/Status.hpp"
#include "Status/floodFill.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Node.hpp"

#ifndef NO_UNSAFE_DIAGNOSTICS
std::size_t Status::copyCount(0);
std::size_t Status::moveCount(0);
std::size_t Status::calculateReachableCount(0);
#endif
std::size_t Status::statusPoolSize_(0);
std::unordered_map<State, std::shared_ptr<Array<Status::CalculatedDataPtr>>>
        Status::statusPool_;
boost::mutex Status::statusPoolMutex_;

Status::Status(const Table& table):
    table_(&table),
    state_(),
    fields_(table.width(), table.height())
{
    init();
}


Status::Status(const Table& table, const State& state, Point currentPos):
    table_(&table),
    state_(state),
    currentPos_(currentPos),
    fields_(table.width(), table.height())
{
    init();
}

Status::Status(const Table& table, const Node& node):
        table_(&table),
        state_(node.state()),
        currentPos_(node.from()),
        fields_(table.width(), table.height())
{
    init();
}

void Status::init() {
    Point p;
    for (Point  p: arrayRange(table())) {
        fields_[p] =
                table().wall(p) ? FieldType::wall : FieldType::floor;
    }
    for (State::const_iterator it = state_.begin(); it != state_.end(); ++it) {
        fields_[*it] = FieldType::stone;
    }
}

void Status::calculateReachable() const
{
#ifndef NO_UNSAFE_DIAGNOSTICS
    ++calculateReachableCount;
#endif
    calculatedData_.reset(new CalculatedData(width(), height()));
    floodFill(*this, currentPos_, calculatedData_->reachable_,
            calculatedData_->border_);
}

void Status::fillReachable() const
{
    if (statusPoolSize_ > 0) {
        std::shared_ptr<Array<CalculatedDataPtr>> poolElement;
        {
            boost::unique_lock<boost::mutex> lock{statusPoolMutex_};
            auto poolIterator = statusPool_.find(state_);
            if (poolIterator != statusPool_.end()) {
                poolElement = poolIterator->second;
                auto storedData = (*poolElement)[currentPos_];
                if (storedData) {
                    calculatedData_ = storedData;
                    return;
                }
            }
        }

        calculateReachable();

        {
            boost::unique_lock<boost::mutex> lock{statusPoolMutex_};
            // if the array doesn't exist in the map, insert it
            if (!poolElement) {
                poolElement = statusPool_.emplace(state_, std::make_shared<
                        Array<CalculatedDataPtr>>(width(), height())).
                                first->second;
            }

            for (Point  p: arrayRange(table())) {
                if (reachable(p)) {
                    (*poolElement)[p] = calculatedData_;
                }
            }

            while (statusPool_.size() > statusPoolSize_) {
                statusPool_.erase(statusPool_.begin());
            }
        }
    } else {
        calculateReachable();
    }
}

bool Status::addStone(Point p) {
    if (value(p) != FieldType::floor)
        return false;
    fields_[p] = FieldType::stone;
    state_.addStone(p);
    calculatedData_.reset();
    return true;
}

bool Status::removeStone(Point p) {
    if (value(p) != FieldType::stone)
        return false;
    fields_[p] = FieldType::floor;
    state_.removeStone(p);
    calculatedData_.reset();
    return true;
}

void Status::state(const State& value) {
    state_ = value;
    calculatedData_.reset();
    init();
}

bool Status::currentPos(Point  p) {
    currentPos_ = p;
    if (calculatedData_ && !calculatedData_->reachable_[p]) {
        calculatedData_.reset();
    }
    return true;
}

bool Status::moveStone(Point from, Point to) {
    if (value(from) != FieldType::stone || value(to) != FieldType::floor)
        return false;
    fields_[from] = FieldType::floor;
    currentPos_ = from;
    state_.removeStone(from);
    if (to != table().destination())
    {
        fields_[to] = FieldType::stone;
        state_.addStone(to);
    }
    calculatedData_.reset();
    return true;
}

void Status::set(const Node& node) {
    state_ = node.state();
    currentPos_ = node.from();
    calculatedData_.reset();
    init();
}

bool Status::shiftIter(Point p)
{
    if (value(p) == FieldType::floor && reachable(p)) {
        currentPos_ = p;
        return true;
    }
    return false;
}

void Status::shiftCurrentPos()
{
    Point p = currentPos();
    if (shiftIter(p+p01))
        return;
    if (shiftIter(p-p01))
        return;
    if (shiftIter(p+p10))
        return;
    if (shiftIter(p-p10))
        return;
}

std::ostream& operator<<(std::ostream& os, const Status& status)
{
    dumpStatus(os, status, " ");
    return os;
}
