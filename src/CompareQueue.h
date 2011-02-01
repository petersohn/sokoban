#ifndef COMPAREQUEUE_H_
#define COMPAREQUEUE_H_

template <class Object, class Value=int>
class CompareQueue {
public:
	typedef Object object_type;
	typedef Value value_type;
	class ValueItf {
	public:
		virtual Value compare(const Object& o1, const Object& o2) = 0;
	};

	template <class Iterator> // Iterator must point to ValueItf* elements
	static Value compareItems(Iterator first, Iterator last, const Object& o1, Object& o2);
};

template <class Object, class Value, class Iterator>
Value CompareQueue::compare(Iterator first, Iterator last, const Object& o1, Object& o2) {
	while (first != last) {
		Value val = (*first)->compare(o1, o2);
		if (value != 0)
			return value;
		++first;
	}
	return value;
}

#endif /* COMPAREQUEUE_H_ */
