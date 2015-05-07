// ConsoleApplication1.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <vector>

template<typename TIterator>
struct from_range
{
	typedef typename std::iterator_traits<TIterator>::value_type value_type;

	TIterator current, upcoming, end;
	from_range(TIterator begin, TIterator end)
		: current(begin), upcoming(begin), end(end) {}

	template<typename TRangeBuilder>
	decltype(auto) operator>>(TRangeBuilder builder) const { return builder.build(*this); }
	decltype(auto) front() const { return *current; }
	bool next() { return upcoming == end ? false : (current = upcoming++, true); }
};

template<typename TRange, typename TPredicate>
struct where_range
{
	typedef typename TRange::value_type value_type;

	TRange range;
	TPredicate predicate;
	where_range(TRange range, TPredicate predicate)
		: range(range), predicate(predicate) {}

	template<typename TRangeBuilder>
	decltype(auto) operator>>(TRangeBuilder builder) const { return builder.build(*this); }
	decltype(auto) front() const { return range.front(); }
	bool next(){
		while(range.next())
			if(predicate(range.front()))
				return true;
		return false;
	}
};

template<typename TPredicate>
struct where_builder
{
	TPredicate predicate;
	where_builder(TPredicate predicate) : predicate(predicate) {}
	template<typename TRange>
	auto build(TRange range){
		return where_range<TRange, TPredicate>(range, predicate);
	}
};

struct to_vector_builder
{
	template<typename TRange>
	auto build(TRange range){
		std::vector<TRange::value_type> result;
		while(range.next())
			result.push_back(range.front());
		return result;
	}
};

template<typename TContainer>
auto from(TContainer const& container)
{
	return from_range<decltype(std::begin(container))>(std::begin(container), std::end(container));
}
auto to_vector() { return to_vector_builder(); }
template<typename TPredicate>
auto where(TPredicate predicate) { return where_builder<TPredicate>(predicate); }

int main()
{
	int a[] = {1, 2, 3, 4};
	auto v = from(a) >> where([](int n){return n % 2 == 0;}) >> to_vector();
	for(int i : v) std::cout << i << std::endl;
}

/*
2
4
*/
