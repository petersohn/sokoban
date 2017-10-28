#include "PrioQueue.hpp"

#include <turtle/mock.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <vector>

using namespace sokoban;

struct Dummy {
    int i;
    Dummy(): i(0) {}
    explicit Dummy(int i): i(i) {}

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & i;
    }
};

std::ostream& operator<<(std::ostream& os, const Dummy& dummy)
{
    return os << dummy.i;
}

bool operator==(const Dummy& lhs, const Dummy& rhs)
{
    return lhs.i == rhs.i;
}

struct MockSerializer {
    MOCK_METHOD(load, 0, void())
    MOCK_METHOD(save, 0, void())
};

class DummyCompare {
public:
    explicit DummyCompare(
            const std::shared_ptr<MockSerializer>& mockSerializer = {}):
            mockSerializer(mockSerializer)
    {}

    bool operator()(const std::shared_ptr<Dummy>& lhs,
            const std::shared_ptr<Dummy>& rhs) {
        return lhs->i > rhs->i;
    }

    template <typename Archive>
    void load(Archive& /*ar*/, const unsigned int /*version*/)
    {
        if (mockSerializer) {
            mockSerializer->load();
        }
    }

    template <typename Archive>
    void save(Archive& /*ar*/, const unsigned int /*version*/) const
    {
        if (mockSerializer) {
            mockSerializer->save();
        }
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
    std::shared_ptr<MockSerializer> mockSerializer;
};

struct PrioQueueTestFixture {
    using PrioQueue = PrioQueue<std::shared_ptr<Dummy>, DummyCompare>;
    std::shared_ptr<MockSerializer> mockSerializer =
            std::make_shared<MockSerializer>();
    PrioQueue queueUnderTest{DummyCompare{mockSerializer}};
};

BOOST_FIXTURE_TEST_SUITE(PrioQueueTest, PrioQueueTestFixture)

BOOST_AUTO_TEST_CASE(empty_at_the_beginning)
{
    BOOST_CHECK_EQUAL(queueUnderTest.size(), 0);
    auto peeked = queueUnderTest.peek();
    BOOST_CHECK_EQUAL(peeked.get(), (Dummy*)nullptr);
    auto popped = queueUnderTest.pop();
    BOOST_CHECK_EQUAL(popped.get(), (Dummy*)nullptr);
}

BOOST_AUTO_TEST_CASE(add_and_peek_one_value)
{

    auto value = std::make_shared<Dummy>(1);
    queueUnderTest.push(value);
    BOOST_CHECK_EQUAL(queueUnderTest.size(), 1);
    auto peeked = queueUnderTest.peek();
    BOOST_CHECK_EQUAL(peeked.get(), value.get());
    BOOST_CHECK_EQUAL(queueUnderTest.size(), 1);
}

BOOST_AUTO_TEST_CASE(add_and_pop_one_value)
{
    auto value = std::make_shared<Dummy>(1);
    queueUnderTest.push(value);
    BOOST_CHECK_EQUAL(queueUnderTest.size(), 1);
    auto peeked= queueUnderTest.pop();
    BOOST_CHECK_EQUAL(peeked.get(), value.get());
    BOOST_CHECK_EQUAL(queueUnderTest.size(), 0);
}

BOOST_AUTO_TEST_CASE(add_and_pop_more_values)
{
    std::vector<std::shared_ptr<Dummy>> values = {
        std::make_shared<Dummy>(1),
        std::make_shared<Dummy>(5),
        std::make_shared<Dummy>(-5),
        std::make_shared<Dummy>(2)};

    for (const auto& value: values) {
        queueUnderTest.push(value);
    }

    BOOST_CHECK_EQUAL(queueUnderTest.pop().get(), values[2].get());
    BOOST_CHECK_EQUAL(queueUnderTest.pop().get(), values[0].get());
    BOOST_CHECK_EQUAL(queueUnderTest.pop().get(), values[3].get());
    BOOST_CHECK_EQUAL(queueUnderTest.pop().get(), values[1].get());
    BOOST_CHECK_EQUAL(queueUnderTest.pop().get(), (Dummy*)nullptr);
}

BOOST_AUTO_TEST_CASE(serialize)
{
    std::vector<std::shared_ptr<Dummy>> values = {
        std::make_shared<Dummy>(4),
        std::make_shared<Dummy>(6),
        std::make_shared<Dummy>(0)};

    for (const auto& value: values) {
        queueUnderTest.push(value);
    }

    mock::sequence seq;
    MOCK_EXPECT(mockSerializer->save).in(seq);
    MOCK_EXPECT(mockSerializer->load).in(seq);

    std::stringstream stream;
    boost::archive::text_oarchive saver{stream};
    saver << queueUnderTest;

    boost::archive::text_iarchive loader{stream};
    PrioQueue loadedQueue{DummyCompare{mockSerializer}};
    loader >> loadedQueue;

    BOOST_CHECK_EQUAL(*loadedQueue.pop(), *values[2]);
    BOOST_CHECK_EQUAL(*loadedQueue.pop(), *values[0]);
    BOOST_CHECK_EQUAL(*loadedQueue.pop(), *values[1]);
    BOOST_CHECK_EQUAL(loadedQueue.pop().get(), (Dummy*)nullptr);
}

BOOST_AUTO_TEST_CASE(serialize_as_pointer)
{
    std::vector<std::shared_ptr<Dummy>> values = {
        std::make_shared<Dummy>(4),
        std::make_shared<Dummy>(6),
        std::make_shared<Dummy>(0)};

    auto originalQueue = std::make_unique<PrioQueue>();
    for (const auto& value: values) {
        originalQueue->push(value);
    }

    std::stringstream stream;
    boost::archive::text_oarchive saver{stream};
    saver << originalQueue;

    boost::archive::text_iarchive loader{stream};
    std::unique_ptr<PrioQueue> loadedQueue;
    loader >> loadedQueue;

    BOOST_CHECK_EQUAL(*loadedQueue->pop(), *values[2]);
    BOOST_CHECK_EQUAL(*loadedQueue->pop(), *values[0]);
    BOOST_CHECK_EQUAL(*loadedQueue->pop(), *values[1]);
    BOOST_CHECK_EQUAL(loadedQueue->pop().get(), (Dummy*)nullptr);
}

BOOST_AUTO_TEST_SUITE_END()

