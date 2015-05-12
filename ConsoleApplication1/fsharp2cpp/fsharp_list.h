#include <iostream>
#include <list>
#include <string>
#include <numeric>
#include <algorithm>
#include <boost/tuple/tuple.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/optional/optional.hpp>
#include <boost/spirit/home/karma.hpp>
namespace fsharp_list
{
	using namespace std;
	using boost::optional;
	using boost::tuple;
	using boost::make_tuple;
	//List.append : 'T list -> 'T list -> 'T list
	//List.append list1 list2
	template<typename T>
	list<T> append(list<T> list1, list<T> const& list2)
	{
		list1.insert(list1.end(), list2.begin(), list2.end());
		return list1;
	}
	//List.average : ^T list -> ^T (requires ^T with static member (+) and ^T with static member DivideByInt and ^T with static member Zero)
	//List.average list
	template<typename T>
	T sum(list<T> const& list1);
	template<typename T>
	T average(list<T> const& list1)
	{
		return sum(list1) / list1.size();
	}
	//List.averageBy : ('T -> ^U) -> 'T list -> ^U (requires ^U with static member (+) and ^U with static member DivideByInt and ^U with static member Zero)
	//List.averageBy projection list
	template<typename Fn, typename T>
	auto sumBy(Fn projection, list<T> const& list1) -> decltype(projection(T()));
	template<typename Fn, typename T>
	auto averageBy(Fn projection, list<T> const& list1) -> decltype(projection(T()))
	{
		return sumBy(projection, list1) / list1.size();
	}
	//List.choose : ('T -> 'U option) -> 'T list -> 'U list
	//List.choose chooser list
	template<typename Fn, typename T>
	auto choose(Fn chooser, list<T> const& list1) -> list<typename remove_reference<decltype(*chooser(T()))>::type>
	{
		list<typename remove_reference<decltype(*chooser(T()))>::type> list_result;
		for_each(list1.begin(), list1.end(), 
			[&](T elem){
				if(auto v = chooser(elem))
					list_result.push_back(*v);
			}
		);
		return list_result;
	}
	//List.collect : ('T -> 'U list) -> 'T list -> 'U list
	//List.collect mapping list
	template<typename Fn, typename T>
	auto collect(Fn mapping, list<T> list1) -> decltype(mapping(T()))
	{
		decltype(mapping(T())) list_result;
		for_each(list1.begin(), list1.end(),
			[&](T elem){
				auto listU = mapping(elem);
				list_result.insert(list_result.end(), listU.begin(), listU.end());
			}
		);
		return list_result;
	}
	//List.empty<'T> :  'T list
	//List.empty
	template<typename T>
	list<T> empty()
	{
		return list<T>();
	}
	//List.exists : ('T -> bool) -> 'T list -> bool
	//List.exists predicate list
	template<typename Fn, typename T>
	bool exists(Fn predicate, list<T> const& list1)
	{
		return any_of(list1.begin(), list1.end(), predicate);
	}
	//List.exists2 : ('T1 -> 'T2 -> bool) -> 'T1 list -> 'T2 list -> bool
	//List.exists2 predicate list1 list2
	template<typename Fn2, typename T1, typename T2>
	bool exists2(Fn2 predicate, list<T1> const& list1, list<T2> const& list2)
	{
		return !equal(list1.begin(), list1.end(), list2.begin(), 
			[&](T1 elem1, T2 elem2){return !predicate(elem1, elem2);});
	}
	//List.find : ('T -> bool) -> 'T list -> 'T
	//List.find predicate list
	template<typename Fn, typename T>
	T find(Fn predicate, list<T> const& list1)
	{
		auto i = find_if(list1.begin(), list1.end(), predicate);
		if(i == list1.end())
			throw runtime_error("");
		return *i;
	}
	//List.findIndex : ('T -> bool) -> 'T list -> int
	//List.findIndex predicate list
	template<typename Fn, typename T>
	size_t findIndex(Fn predicate, list<T> const& list1)
	{
		size_t index = 0;
		auto i = find_if(list1.begin(), list1.end(), 
			[&](T elem){return predicate(elem) ? true : (++index, false);});
		if(i == list1.end())
			throw runtime_error("");
		return index;
	}
	//List.filter : ('T -> bool) -> 'T list -> 'T list
	//List.filter predicate list
	template<typename Fn, typename T>
	list<T> filter(Fn predicate, list<T> list1)
	{
		list1.remove_if([&](T elem){return !predicate(elem);});
		return list1;
	}
	//List.fold : ('State -> 'T -> 'State) -> 'State -> 'T list -> 'State
	//List.fold folder state list
	template<typename Fn2, typename State, typename T>
	State fold(Fn2 folder, State state, list<T> const& list1)
	{
		return accumulate(list1.begin(), list1.end(), state, folder);
	}
	//List.fold2 : ('State -> 'T1 -> 'T2 -> 'State) -> 'State -> 'T1 list -> 'T2 list -> 'State
	//List.fold2 folder state list1 list2
	template<typename Fn3, typename State, typename T1, typename T2>
	State fold2(Fn3 folder, State state, list<T1> const& list1, list<T2> const& list2)
	{
		auto first2 = list2.begin();
		return accumulate(list1.begin(), list1.end(), state,
			[&](State state, T1 elem1) -> State {return folder(state, elem1, *first2++);});
	}
	//List.foldBack : ('T -> 'State -> 'State) -> 'T list -> 'State -> 'State
	//List.foldBack folder list state
	template<typename Fn2, typename T, typename State>
	State foldBack(Fn2 folder, list<T> const& list1, State state)
	{
		return accumulate(list1.rbegin(), list1.rend(), state, 
			[&](State state, T elem) -> State {return folder(elem, state);});
	}
	//List.foldBack2 : ('T1 -> 'T2 -> 'State -> 'State) -> 'T1 list -> 'T2 list -> 'State -> 'State
	//List.foldBack2 folder list1 list2 state
	template<typename Fn3, typename T1, typename T2, typename State>
	State foldBack2(Fn3 folder, list<T1> const& list1, list<T2> const& list2, State state)
	{
		auto first2 = list2.rbegin();
		return accumulate(list1.rbegin(), list1.rend(), state, 
			[&](State state, T1 elem1) -> State {return folder(elem1, *first2++, state);});
	}
	//List.forall : ('T -> bool) -> 'T list -> bool
	//List.forall predicate list	
	template<typename Fn, typename T>
	bool forall(Fn predicate, list<T> const& list1)
	{
		return all_of(list1.begin(), list1.end(), predicate);
	}
	//List.forall2 : ('T1 -> 'T2 -> bool) -> 'T1 list -> 'T2 list -> bool
	//List.forall2 predicate list1 list2
	template<typename Fn2, typename T1, typename T2>
	bool forall2(Fn2 predicate, list<T1> const& list1, list<T2> const& list2)
	{
		return equal(list1.begin(), list1.end(), list2.begin(), predicate);
	}
	//List.head : 'T list -> 'T
	//List.head list
	template<typename T>
	T head(list<T> const& list1)
	{
		return list1.front();
	}
	//List.init : int -> (int -> 'T) -> 'T list
	//List.init length initializer
	template<typename Fn>
	auto init(size_t length, Fn initializer) -> list<decltype(initializer(size_t()))>
	{
		list<decltype(initializer(size_t()))> list_result(length);
		size_t index = 0;
		generate(list_result.begin(), list_result.end(), [&]{return initializer(index++);});
		return list_result;
	}
	//List.isEmpty : 'T list -> bool
	//List.isEmpty list
	template<typename T>
	bool isEmpty(list<T> const& list1)
	{
		return list1.empty();
	}
	//List.iter : ('T -> unit) -> 'T list -> unit
	//List.iter action list
	template<typename Fn, typename T>
	void iter(Fn action, list<T> const& list1)
	{
		for_each(list1.begin(), list1.end(), action);
	}
	//List.iter2 : ('T1 -> 'T2 -> unit) -> 'T1 list -> 'T2 list -> unit
	//List.iter2 action list1 list2	
	template<typename Fn2, typename T1, typename T2>
	void iter2(Fn2 action, list<T1> const& list1, list<T2> const& list2)
	{
		auto first2 = list2.begin();
		for_each(list1.begin(), list1.end(), [&](T elem){action(elem, *first2++);});
	}
	//List.iteri : (int -> 'T -> unit) -> 'T list -> unit
	//List.iteri action list
	template<typename Fn2, typename T>
	void iteri(Fn2 action, list<T> const& list1)
	{
		size_t index = 0;
		for_each(list1.begin(), list1.end(), [&](T elem){action(index++, elem);});
	}
	//List.iteri2 : (int -> 'T1 -> 'T2 -> unit) -> 'T1 list -> 'T2 list -> unit
	//List.iteri2 action list1 list2
	template<typename Fn3, typename T1, typename T2>
	void iteri2(Fn3 action, list<T1> const& list1, list<T2> const& list2)
	{
		auto first2 = list2.begin();
		size_t index = 0;
		for_each(list1.begin(), list1.end(), [&](T elem){action(index++, elem, *first2++);});
	}
	//List.length : 'T list -> int
	//List.length list
	template<typename T>
	size_t length(list<T> const& list1)
	{
		return list1.size();
	}
	//List.map : ('T -> 'U) -> 'T list -> 'U list
	//List.map mapping list
	template<typename Fn, typename T>
	auto map(Fn mapping, list<T> const& list1) -> list<decltype(mapping(T()))>
	{
		list<decltype(mapping(T()))> list_result(list1.size());
		transform(list1.begin(), list1.end(), list_result.begin(), mapping);
		return list_result;
	}
	//List.map2 : ('T1 -> 'T2 -> 'U) -> 'T1 list -> 'T2 list -> 'U list
	//List.map2 mapping list1 list2
	template<typename Fn2, typename T1, typename T2>
	auto map2(Fn2 mapping, list<T1> const& list1, list<T2> const& list2) -> list<decltype(mapping(T1(), T2()))>
	{
		list<decltype(mapping(T1(), T2()))> list_result(list1.size());
		transform(list1.begin(), list1.end(), list2.begin(), list_result.begin(), mapping);
		return list_result;
	}
	//List.map3 : ('T1 -> 'T2 -> 'T3 -> 'U) -> 'T1 list -> 'T2 list -> 'T3 list -> 'U list
	//List.map3 mapping list1 list2 list3
	template<typename Fn2, typename T1, typename T2, typename T3>
	auto map3(Fn2 mapping, list<T1> const& list1, list<T2> const& list2, list<T3> const& list3)
		-> list<decltype(mapping(T1(), T2(), T3()))>
	{
		list<decltype(mapping(T1(), T2(), T3()))> list_result(list1.size());
		auto first3 = list3.begin();
		transform(list1.begin(), list1.end(), list2.begin(), list_result.begin(), 
			[&](T1 elem1, T2 elem2){return mapping(elem1, elem2, *first3++);});
		return list_result;
	}
	//List.mapi : (int -> 'T -> 'U) -> 'T list -> 'U list
	//List.mapi mapping list
	template<typename Fn2, typename T>
	auto mapi(Fn2 mapping, list<T> const& list1) -> list<decltype(mapping(size_t(), T()))>
	{
		list<decltype(mapping(size_t(), T()))> list_result(list1.size());
		size_t index = 0;
		transform(list1.begin(), list1.end(), list_result.begin(), 
			[&](T elem){return mapping(index++, elem);});
		return list_result;
	}
	//List.mapi2 : (int -> 'T1 -> 'T2 -> 'U) -> 'T1 list -> 'T2 list -> 'U list
	//List.mapi2 mapping list1 list2
	template<typename Fn3, typename T1, typename T2>
	auto mapi2(Fn3 mapping, list<T1> const& list1, list<T2> const& list2)
		-> list<decltype(size_t(), mapping(T1(), T2()))>
	{
		list<decltype(mapping(size_t(), T1(), T2()))> list_result(list1.size());
		size_t index = 0;
		transform(list1.begin(), list1.end(), list2.begin(), list_result.begin(), 
			[&](T1 elem1, T2 elem2){return mapping(index++, elem1, elem2);});
		return list_result;
	}
	//List.max : 'T list -> 'T (requires comparison)
	//List.max list
	template<typename T>
	T max(const list<T>& list1)
	{
		return *max_element(list1.begin(), list1.end());
	}
	//List.maxBy : ('T -> 'U) -> 'T list -> 'T (requires comparison)
	//List.maxBy projection list
	template<typename Fn, typename T>
	T maxBy(Fn projection, list<T> const& list1)
	{
		return *max_element(list1.begin(), list1.end(),
			[&](T elem1, T elem2){return projection(elem1) < projection(elem2);});
	}
	//List.min : 'T list -> 'T (requires comparison)
	//List.min list
	template<typename T>
	T min(list<T> const& list1)
	{
		return *min_element(list1.begin(), list1.end());
	}
	//List.minBy : ('T -> 'U) -> 'T list -> 'T (requires comparison)
	//List.minBy projection list
	template<typename Fn, typename T>
	T minBy(Fn projection, list<T> const& list1)
	{
		return *min_element(list1.begin(), list1.end(),
			[&](T elem1, T elem2){return projection(elem1) < projection(elem2);});
	}
	//List.nth : 'T list -> int -> 'T
	//List.nth list index
	template<typename T>
	T nth(list<T> const& list1, size_t index)
	{
		return *next(list1.begin(), index);
	}
	//List.partition : ('T -> bool) -> 'T list -> 'T list * 'T list
	//List.partition predicate list
	template<typename Fn, typename T>
	tuple<list<T>, list<T>> partition(Fn predicate, list<T> const& list1)
	{
		list<T> list2, list3;
		for_each(list1.begin(), list1.end(), 
			[&](T elem){predicate(elem) ? list2.push_back(elem) : list3.push_back(elem);}
		);
		return make_tuple(list2, list3);
	}
	//List.permute : (int -> int) -> 'T list -> 'T list
	//List.permute indexMap list
	template<typename Fn, typename T>
	list<T> permute(Fn indexMap, list<T> const& list1)
	{
		list<T> list_result(list1.size());
		size_t index = 0;
		for_each(list1.begin(), list1.end(), [&](T elem){
			*next(list_result.begin(), indexMap(index++)) = elem;
		});
		return list_result;
	}
	//List.pick : ('T -> 'U option) -> 'T list -> 'U
	//List.pick chooser list
	template<typename Fn, typename T>
	auto pick(Fn chooser, list<T> const& list1) -> typename remove_reference<decltype(*chooser(T()))>::type
	{
		decltype(chooser(T())) opt;
		auto i = find_if(list1.begin(), list1.end(), 
			[&](T elem) -> bool{return opt = chooser(elem);});
		if(i == list1.end())
			throw runtime_error("");
		return *opt;
	}
	//List.reduce : ('T -> 'T -> 'T) -> 'T list -> 'T
	//List.reduce reduction list
	template<typename Fn2, typename T>
	T reduce(Fn2 reduction, list<T> const& list1)
	{
		if(list1.empty())
			throw runtime_error("");
		return accumulate(++list1.begin(), list1.end(), list1.front(), reduction);
	}
	//List.reduceBack : ('T -> 'T -> 'T) -> 'T list -> 'T
	//List.reduceBack reduction list
	template<typename Fn2, typename T>
	T reduceBack(Fn2 reduction, list<T> const& list1)
	{
		if(list1.empty())
			throw runtime_error("");
		return accumulate(++list1.rbegin(), list1.rend(), list1.back(), 
			[&](T state, T elem) -> T {return reduction(elem, state);});
	}
	//List.replicate : int -> 'T -> 'T list
	//List.replicate count initial
	template<typename T>
	list<T> replicate(size_t count, T initial)
	{
		return list<T>(count, initial);
	}
	//List.rev : 'T list -> 'T list
	//List.rev list
	template<typename T>
	list<T> rev(list<T> list1)
	{
		return list1.reverse(), list1;
	}
	//List.scan : ('State -> 'T -> 'State) -> 'State -> 'T list -> 'State list
	//List.scan folder state list
	template<typename Fn2, typename State, typename T>
	list<State> scan(Fn2 folder, State state, list<T> const& list1)
	{
		list<State> list_result(list1.size() + 1);
		auto first2 = list_result.begin();
		*first2++ = state;
		for_each(list1.begin(), list1.end(), 
			[&](T elem){*first2++ = state = folder(state, elem);});
		return list_result;
	}
	//List.scanBack : ('T -> 'State -> 'State) -> 'T list -> 'State -> 'State list
	//List.scanBack folder list state
	template<typename Fn2, typename T, typename State>
	list<State> scanBack(Fn2 folder, list<T> const& list1, State state)
	{
		list<State> list_result(list1.size() + 1);
		auto first2 = list_result.rbegin();
		*first2++ = state;
		for_each(list1.rbegin(), list1.rend(), 
			[&](T elem){*first2++ = state = folder(elem, state);});
		return list_result;
	}
	//List.sortWith : ('T -> 'T -> int) -> 'T list -> 'T list
	//List.sortWith comparer list
	template<typename Fn2, typename T>
	list<T> sortWith(Fn2 comparer, list<T> list1)
	{
		return list1.sort([&](T elem1, T elem2){return comparer(elem1, elem2) < 0;}), list1;
	}
	//List.sortBy : ('T -> 'Key) -> 'T list -> 'T list (requires comparison)
	//List.sortBy projection list
	template<typename Fn, typename T>
	list<T> sortBy(Fn projection, list<T> list1)
	{
		return list1.sort([&](T elem1, T elem2){return projection(elem1) < projection(elem2);}), list1;
	}
	//List.sort : 'T list -> 'T list (requires comparison)
	//List.sort list
	template<typename T>
	list<T> sort(list<T> list1)
	{
		return list1.sort(), list1;
	}
	//List.sum : ^T list -> ^T (requires ^T with static member (+) and ^T with static member Zero)
	//List.sum list
	template<typename T>
	T sum(list<T> const& list1)
	{
		return accumulate(list1.begin(), list1.end(), T());
	}
	//List.sumBy : ('T -> ^U) -> 'T list -> ^U (requires ^U with static member (+) and ^U with static member Zero)
	//List.sumBy projection list
	template<typename Fn, typename T>
	auto sumBy(Fn projection, list<T> const& list1) -> decltype(projection(T()))
	{
		typedef decltype(projection(T())) U;
		return accumulate(list1.begin(), list1.end(), U(),
			[&](U acc, T elem){return acc + projection(elem);});
	}
	//List.tail : 'T list -> 'T list
	//List.tail list
	template<typename T>
	list<T> tail(list<T> list1)
	{
		return list1.erase(list1.begin()), list1;
	}
	//List.tryPick : ('T -> 'U option) -> 'T list -> 'U option
	//List.tryPick chooser list
	template<typename Fn, typename T>
	auto tryPick(Fn chooser, list<T> const& list1) -> decltype(chooser(T()))
	{
		decltype(chooser(T())) opt;
		find_if(list1.begin(), list1.end(), 
			[&](T elem) -> bool{return opt = chooser(elem);});
		return opt;
	}
	//List.tryFind : ('T -> bool) -> 'T list -> 'T option
	//List.tryFind predicate list
	template<typename Fn, typename T>
	optional<T> tryFind(Fn predicate, list<T> const& list1)
	{
		auto i = find_if(list1.begin(), list1.end(), predicate);
		return i == list1.end() ? optional<T>() : optional<T>(*i);
	}
	//List.tryFindIndex : ('T -> bool) -> 'T list -> int option
	//List.tryFindIndex predicate list
	template<typename Fn, typename T>
	optional<size_t> tryFindIndex(Fn predicate, list<T> const& list1)
	{
		size_t index = 0;
		auto i = find_if(list1.begin(), list1.end(), 
			[&](T elem){return predicate(elem) ? true : (++index, false);});
		return i == list1.end() ? optional<size_t>() : optional<size_t>(index);
	}
	//List.zip : 'T1 list -> 'T2 list -> 'T1 * 'T2 list
	//List.zip list1 list2
	template<typename T1, typename T2>
	list<tuple<T1, T2>> zip(list<T1> const& list1, list<T2> const& list2)
	{
		list<tuple<T1, T2>> list_result(list1.size());
		transform(list1.begin(), list1.end(), list2.begin(), list_result.begin(), 
			[&](T1 elem1, T2 elem2){return make_tuple(elem1, elem2);});
		return list_result;
	}
	//List.zip3 : 'T1 list -> 'T2 list -> 'T3 list -> 'T1 * 'T2 * 'T3 list
	//List.zip3 list1 list2 list3
	template<typename T1, typename T2, typename T3>
	list<tuple<T1, T2, T3>> zip3(list<T1> const& list1, list<T2> const& list2, list<T3> const& list3)
	{
		list<boost::tuple<T1, T2, T3>> list_result(list1.size());
		auto first3 = list3.begin();
		transform(list1.begin(), list1.end(), list2.begin(), list_result.begin(), 
			[&](T1 elem1, T2 elem2){return make_tuple(elem1, elem2, *first3++);});
		return list_result;
	}
	//List.unzip : 'T1 * 'T2 list -> 'T1 list * 'T2 list
	//List.unzip list
	template<typename T1, typename T2>
	tuple<list<T1>, list<T2>> unzip(list<tuple<T1, T2>> const& list1)
	{
		list<T1> list2;
		list<T2> list3;
		for_each(list1.begin(), list1.end(), [&](tuple<T1, T2> const& t){
			list2.push_back(get<0>(t));
			list3.push_back(get<1>(t));
		});
		return make_tuple(list2, list3);
	}
	//List.unzip3 : 'T1 * 'T2 * 'T3 list -> 'T1 list * 'T2 list * 'T3 list
	//List.unzip3 list
	template<typename T1, typename T2, typename T3>
	tuple<list<T1>, list<T2>, list<T3>> unzip3(list<tuple<T1, T2, T3>> const& list1)
	{
		list<T1> list2;
		list<T2> list3;
		list<T3> list4;
		for_each(list1.begin(), list1.end(), [&](tuple<T1, T2, T3> const& t){
			list2.push_back(get<0>(t));
			list3.push_back(get<1>(t));
			list4.push_back(get<2>(t));
		});
		return make_tuple(list2, list3, list4);
	}
}

namespace std
{
	namespace karma = boost::spirit::karma;
	template<typename T>
	ostream& operator<<(ostream& os, list<T> const& list1)
	{
		return os << karma::format('[' << -(karma::auto_ % "; ") << ']', list1);
	}
	template<typename T1, typename T2>
	ostream& operator<<(ostream& os, boost::tuple<T1, T2> const& list1)
	{
		return os << karma::format(karma::auto_, list1);
	}
	template<typename T1, typename T2, typename T3>
	ostream& operator<<(ostream& os, boost::tuple<T1, T2, T3> const& list1)
	{
		return os << karma::format(karma::auto_, list1);
	}
}

namespace boost { namespace spirit { namespace traits
{
	template <typename T>
	struct create_generator<std::list<T>>
	{
		typedef proto::result_of::deep_copy<
			BOOST_TYPEOF('[' << -(karma::auto_ % "; ") << ']')
		>::type type;

		static type call()
		{
			return proto::deep_copy(
				'[' << -(karma::auto_ % "; ") << ']');
		}
	};
	template <typename T1, typename T2>
	struct create_generator<boost::tuple<T1, T2>>
	{
		typedef proto::result_of::deep_copy<
			BOOST_TYPEOF('(' << karma::auto_ << ", " << karma::auto_ << ')')
		>::type type;

		static type call()
		{
			return proto::deep_copy(
				'(' << karma::auto_ << ", " << karma::auto_ << ')');
		}
	};
	template <typename T1, typename T2, typename T3>
	struct create_generator<boost::tuple<T1, T2, T3>>
	{
		typedef proto::result_of::deep_copy<
			BOOST_TYPEOF('(' << karma::auto_ << ", " << karma::auto_ << ", " << karma::auto_ << ')')
		>::type type;

		static type call()
		{
			return proto::deep_copy(
				'(' << karma::auto_ << ", " << karma::auto_ << ", " << karma::auto_ << ')');
		}
	};
}}}
