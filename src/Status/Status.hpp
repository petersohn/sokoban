#ifndef STATUS_H_
#define STATUS_H_

#include "Status/Table.hpp"
#include "Status/State.hpp"
#include "Matrix.hpp"
#include "FieldType.hpp"

#include <boost/thread/mutex.hpp>

#include <deque>
#include <functional>
#include <iosfwd>
#include <memory>
#include <unordered_map>
#include <vector>

namespace sokoban {

class Node;

class Status {
public:
    typedef std::vector<Point> BorderType;
private:
    const Table* table_;
    State state_;
    Point currentPos_;
    Matrix<FieldType> fields_;

    struct CalculatedData {
        Matrix<bool> reachable_;
        BorderType border_;

        CalculatedData(std::size_t width, std::size_t height):
            reachable_(width, height, false)
        {}
    };

    typedef std::shared_ptr<CalculatedData> CalculatedDataPtr;

    mutable  CalculatedDataPtr calculatedData_;

    static std::size_t statusPoolSize_;
    static std::unordered_map<State, std::shared_ptr<Matrix<CalculatedDataPtr>>> statusPool_;
    static boost::mutex statusPoolMutex_;

    void fillReachable() const;
    void calculateReachable() const;
    void init();
    bool shiftIter(Point p);

    const CalculatedData& calculatedData() const
    {
        if (!calculatedData_) {
            fillReachable();
        }

        return *calculatedData_;
    }

public:
#ifndef NO_UNSAFE_DIAGNOSTICS
    static std::size_t copyCount;
    static std::size_t moveCount;
    static std::size_t calculateReachableCount;
#endif

    explicit Status(const Table& table);
    explicit Status(const Table& table, const State& state, Point currentPos);
    explicit Status(const Table& table, const Node& node);
    Status(const Status& other):
        table_(other.table_),
        state_(other.state_),
        currentPos_(other.currentPos_),
        fields_(other.fields_),
        calculatedData_(other.calculatedData_)
    {
#ifndef NO_UNSAFE_DIAGNOSTICS
        ++copyCount;
#endif
    }
    Status& operator=(const Status& other) {
        table_ = other.table_;
        state_ = other.state_;
        currentPos_ = other.currentPos_;
        fields_ = other.fields_;
        calculatedData_ = other.calculatedData_;
#ifndef NO_UNSAFE_DIAGNOSTICS
        ++copyCount;
#endif
        return *this;
    }

    Status(Status&& other) noexcept:
        table_(other.table_),
        state_(std::move(other.state_)),
        currentPos_(std::move(other.currentPos_)),
        fields_(std::move(other.fields_)),
        calculatedData_(std::move(other.calculatedData_))
    {
#ifndef NO_UNSAFE_DIAGNOSTICS
        ++moveCount;
#endif
    }
    Status& operator=(Status&& other) noexcept {
        table_ = other.table_;
        state_ = std::move(other.state_);
        currentPos_ = std::move(other.currentPos_);
        fields_ = std::move(other.fields_);
        calculatedData_ = std::move(other.calculatedData_);
#ifndef NO_UNSAFE_DIAGNOSTICS
        ++moveCount;
#endif
        return *this;
    }

    Status deepCopy() const {
        Status result{*this};
        result.state_ = result.state_.deepCopy();
        result.calculatedData_.reset();
        return result;
    }

    bool operator==(const Status& other) const
    {
        if (table_ != other.table_ || state_ != other.state_) {
            return false;
        }

        if (calculatedData_) {
            return reachable(other.currentPos_);
        } else {
            return other.reachable(currentPos_);
        }
    }

    const Table& table() const { return *table_; }
    std::size_t width() const { return table().width(); }
    std::size_t height() const { return table().height(); }
    const State& state() const { return state_; }
    bool reachable(Point p) const {
        return matrixAt<bool>(calculatedData().reachable_, p, false);
    }
    const Matrix<bool>& reachableMatrix() const {
        return calculatedData().reachable_;
    }
    const BorderType& border() const {
        return calculatedData().border_;
    }
    FieldType value(Point p) const { return matrixAt<FieldType>(fields_, p, FieldType::wall); }
    Point currentPos() const { return currentPos_; }

    bool currentPos(Point p);
    void state(const State& value);
    bool addStone(Point p);
    bool removeStone(Point p);
    bool moveStone(Point from, Point to);
    void set(const Node& node);
    void shiftCurrentPos();

    static void statusPoolSize(std::size_t value) {
        statusPoolSize_ = value;
    }
    static std::size_t statusPoolSize() { return statusPoolSize_; }
    static std::size_t currentStatusPoolSize() { return statusPool_.size(); }
};

inline bool operator!=(const Status& lhs, const Status& rhs)
{
    return !(lhs == rhs);
}

template <typename Archive>
void save(Archive& ar, const Status& status, const unsigned int /*version*/)
{
    ar << status.state();
    Point currentPos = status.currentPos();
    ar << currentPos;
}

template <typename Archive>
void load(Archive& ar, Status& status, const unsigned int /*version*/)
{
    State state;
    ar >> state;
    Point currentPos;
    ar >> currentPos;

    status.state(state);
    status.currentPos(currentPos);
}

template <typename Archive>
void save_construct_data(Archive& ar, const Status* status,
        const unsigned int /*version*/)
{
    const Table* table = &status->table();
    ar << table;
}

template <typename Archive>
void load_construct_data(Archive& ar, Status* status,
        const unsigned int /*version*/)
{
    Table* table;
    ar >> table;
    ::new(status)Status{*table};
}

std::ostream& operator<<(std::ostream& os, const Status& status);

} // namespace sokoban

BOOST_SERIALIZATION_SPLIT_FREE(sokoban::Status)

#endif /* STATUS_H_ */
