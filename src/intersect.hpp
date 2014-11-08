#ifndef SRC_INTERSECT_HPP
#define SRC_INTERSECT_HPP

#include <vector>
#include <queue>

template<class T>
std::vector<T> intersect(const std::vector<T>& l1, const std::vector<T>& l2)
{
	std::priority_queue<T> q1(l1.begin(), l1.end());
	std::priority_queue<T> q2(l2.begin(), l2.end());
	std::vector<T> result;
	T last;
	bool start = true;
	while (!q1.empty() && !q2.empty())
	{
		if (q1.top() == q2.top())
		{
			if (start || last != q1.top())
			{
				result.push_back(q1.top());
				last = q1.top();
				start = false;
			}
			q1.pop();
			q2.pop();
		} else if (q1.top() < q2.top())
			q2.pop();
		else
			q1.pop();
	}
	return result;
}



#endif /* SRC_INTERSECT_HPP */
