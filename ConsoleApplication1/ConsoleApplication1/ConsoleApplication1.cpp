// ConsoleApplication1.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <vector>

template<typename TIterator>
struct from_range
{
	// range接口的成员类型
	// 其含义为C++容器所包含数据的值类型
	typedef typename std::iterator_traits<TIterator>::value_type value_type;

	// from_range内含三个指向数据的迭代器
	// 当前迭代器，其次迭代器，末尾迭代器
	TIterator current, upcoming, end;
	// from_range的构造器
	// 当前迭代器指向容器的开头
	// 其次迭代器指向容器的开头
	// 末尾迭代器指向容器的末尾
	from_range(TIterator begin, TIterator end)
		: current(begin), upcoming(begin), end(end) {}

	// range接口的成员方法
	// 重载>>操作符，该操作符接收一个build对象作为右操作符
	// 该操作符将自身传给builder对象的build方法，通过调用该方法实现对range的转换
	template<typename TRangeBuilder>
	decltype(auto) operator>>(TRangeBuilder builder) const { return builder.build(*this); }
	// front方法，返回当前迭代器所指向的数据，即当前数据
	decltype(auto) front() const { return *current; }
	// next方法，当前数据不存在则返回false，若存在则准备当前数据并返回true
	// 具体实现为
	// 1. 若其次迭代器与末尾迭代器的值相等，则其次迭代器已经指向容器末尾
	//    判断当前数据不存在，返回false
	// 2. 若其次迭代器与末尾迭代器的值不等，则其次迭代器尚未指向容器末尾
	//    将其次迭代器的值赋予当前迭代器，递增其次迭代器，令其指向下一个数据
	//    判断当前数据存在，返回true
	bool next() { return upcoming == end ? false : (current = upcoming++, true); }
};

template<typename TRange, typename TPredicate>
struct where_range
{
	// range接口的成员类型
	// 其含义为转换源range所包含数据的值类型
	typedef typename TRange::value_type value_type;

	// where_range内含两个成员
	// 转换源range，谓词（过滤条件）
	TRange range;
	TPredicate predicate;
	// where_range的构造器
	where_range(TRange range, TPredicate predicate)
		: range(range), predicate(predicate) {}

	// range接口的成员方法
	// 重载>>操作符，该操作符接收一个build对象作为右操作符
	// 该操作符将自身传给builder对象的build方法，通过调用该方法实现对range的转换
	template<typename TRangeBuilder>
	decltype(auto) operator>>(TRangeBuilder builder) const { return builder.build(*this); }
	// front方法，返回转换源range的当前数据
	decltype(auto) front() const { return range.front(); }
	// next方法，当前数据不存在则返回false，若存在则准备当前数据并返回true
	// 具体实现为
	// 反复扫描数据源range直至找到符合过滤条件的下一个数据
	// 若该数据存在，则当前数据存在，返回true
	// 否则当前数据不存在，返回false
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
	// where_builder只包含一个成员：谓词（过滤条件）
	TPredicate predicate;
	// where_builder的构造器
	where_builder(TPredicate predicate) : predicate(predicate) {}

	// builder接口的成员方法
	// build方法，生成并返回where_range对象
	template<typename TRange>
	auto build(TRange range){
		return where_range<TRange, TPredicate>(range, predicate);
	}
};

struct to_vector_builder
{
	// builder接口的成员方法
	// build方法，生成并返回C++容器
	template<typename TRange>
	auto build(TRange range){
		std::vector<TRange::value_type> result;
		while(range.next())
			result.push_back(range.front());
		return result;
	}
};

// from操作符，将C++容器转换成from_range对象
template<typename TContainer>
auto from(TContainer const& container)
{
	return from_range<decltype(std::begin(container))>(std::begin(container), std::end(container));
}
// where操作符，生成并返回where_builder对象
template<typename TPredicate>
auto where(TPredicate predicate) { return where_builder<TPredicate>(predicate); }
// to_vector操作符，生成并返回to_vector_builder对象
auto to_vector() { return to_vector_builder(); }

int main()
{
	int a[] = {1, 2, 3, 4};
	// 找出数组a中所有的偶数，将结果存放到一个vector对象中
	auto v = from(a) >> where([](int n){return n % 2 == 0;}) >> to_vector();
	for(int i : v) std::cout << i << std::endl;
}

/*
2
4
*/
