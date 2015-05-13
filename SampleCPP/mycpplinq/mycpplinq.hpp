#pragma once

/*

/// range接口
struct xxx_range
{
	// range接口的成员类型
	// 其含义为range类的front方法所返回的当前数据的值类型
	using value_type = typename ...;

	// range接口的成员方法
	// 重载>>操作符，该操作符接收一个build对象作为右操作符
	// 该操作符将自身传给builder对象的build方法，通过调用该方法实现对range的转换
	template<typename TRangeBuilder>
	decltype(auto) operator>>(TRangeBuilder builder) const { return builder.build(*this); }
	// front方法，返回当前数据
	decltype(auto) front() const { return ...; }
	// next方法，若当前数据不存在则返回false，若存在则准备当前数据并返回true
	bool next() { return ...; }
};

/// builder接口
struct xxx_builder
{
	// builder接口的成员方法
	// build方法，转换range的种类或者生成C++容器
	template<typename TRange>
	auto build(TRange range){ return ...; }
};

*/

template<typename TIterator>
struct from_range
{
	// range接口的成员类型
	// 其含义为C++容器所包含数据的值类型
	using value_type = typename std::iterator_traits<TIterator>::value_type;

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

template<typename TRange, typename TSelector>
struct select_range
{
	// range接口的成员类型
	// 其含义为转换源range所包含数据经过映射后的值类型
	using value_type = typename decltype(std::declval<TSelector>()(std::declval<TRange::value_type>()));

	// select_range内含两个成员
	// 转换源range，映射函数selector
	TRange range;
	TSelector selector;
	// select_range的构造器
	select_range(TRange range, TSelector selector)
		: range(range), selector(selector) {}

	// range接口的成员方法
	// 重载>>操作符，该操作符接收一个build对象作为右操作符
	// 该操作符将自身传给builder对象的build方法，通过调用该方法实现对range的转换
	template<typename TRangeBuilder>
	decltype(auto) operator>>(TRangeBuilder builder) const { return builder.build(*this); }
	// front方法，返回对转换源range的当前数据进行映射后的结果
	decltype(auto) front() const { return selector(range.front()); }
	// next方法，当前数据不存在则返回false，若存在则准备当前数据并返回true
	// 具体实现为
	// 调用数据源range的next方法
	bool next() { return range.next(); }
};

template<typename TSelector>
struct select_builder
{
	// select_builder只包含一个成员：映射函数selector
	TSelector selector;
	// select_builder的构造器
	select_builder(TSelector selector) : selector(selector) {}

	// builder接口的成员方法
	// build方法，生成并返回select_range对象
	template<typename TRange>
	auto build(TRange range){
		return select_range<TRange, TSelector>(range, selector);
	}
};

template<typename TRange, typename TPredicate>
struct where_range
{
	// range接口的成员类型
	// 其含义为转换源range所包含数据的值类型
	using value_type = typename TRange::value_type;

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
	// 依次扫描数据源range直至找到符合过滤条件的下一个数据
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
	// 具体实现为
	// 依次扫描数据源range
	// 将扫描所得到的数据依次插入到C++容器中
	template<typename TRange>
	auto build(TRange range){
		std::vector<TRange::value_type> result;
		while(range.next())
			result.push_back(range.front());
		return result;
	}
};

// LINQ操作符from，将C++容器转换成from_range对象
template<typename TContainer>
auto from(TContainer const& container)
{
	return from_range<decltype(std::cbegin(container))>(std::cbegin(container), std::cend(container));
}
// LINQ操作符select，生成并返回select_builder对象
template<typename TSelector>
auto select(TSelector selector) { return select_builder<TSelector>(selector); }
// LINQ操作符where，生成并返回where_builder对象
template<typename TPredicate>
auto where(TPredicate predicate) { return where_builder<TPredicate>(predicate); }
// LINQ操作符to_vector，生成并返回to_vector_builder对象
auto to_vector() { return to_vector_builder(); }
