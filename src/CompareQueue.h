#ifndef COMPAREQUEUE_H_
#define COMPAREQUEUE_H_

// Value must be compatible with int
template <class Object, class Value=int>
class CompareQueue {
public:
	typedef Object object_type;
	typedef Value value_type;

	// Iterator must point to function objects that accept two
	// Object arguments, and return a type of Value.
	template <class Iterator>
	static Value compareItems(Iterator first, Iterator last, const Object& o1, const Object& o2);
};

template <class Object, class Value, class Iterator>
Value CompareQueue::compare(Iterator first, Iterator last, const Object& o1, const Object& o2) {
	while (first != last) {
		Value val = (*first)(o1, o2);
		if (value != 0)
			return value;
		++first;
	}
	return value;
}

#endif /* COMPAREQUEUE_H_ */
