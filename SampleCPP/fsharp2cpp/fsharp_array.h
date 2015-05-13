#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <boost/tuple/tuple.hpp>
#include <boost/utility.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/optional/optional.hpp>
#include <boost/spirit/home/karma.hpp>
namespace fsharp_array
{
	using namespace std;
	using boost::optional;
	using boost::tuple;
	using boost::make_tuple;
	//Array.append : 'T [] -> 'T [] -> 'T []
	//Array.append array1 array2
	template<typename T>
	vector<T> append(vector<T> array1, vector<T> const& array2)
	{
		array1.insert(array1.end(), array2.begin(), array2.end());
		return array1;
	}
	//Array.average : ^T [] -> ^T (requires ^T with static member (+) and ^T with static member DivideByInt and ^T with static member Zero)
	//Array.average array
	template<typename T>
	T sum(vector<T> const& array1);
	template<typename T>
	T average(vector<T> const& array1)
	{
		return sum(array1) / array1.size();
	}
	//Array.averageBy : ('T -> ^U) -> 'T [] -> ^U (requires ^U with static member (+) and ^U with static member DivideByInt and ^U with static member Zero)
	//Array.averageBy projection array
	template<typename Fn, typename T>
	auto sumBy(Fn projection, vector<T> const& array1) -> decltype(projection(T()));
	template<typename Fn, typename T>
	auto averageBy(Fn projection, vector<T> const& array1) -> decltype(projection(T()))
	{
		return sumBy(projection, array1) / array1.size();
	}
	//Array.blit : 'T [] -> int -> 'T [] -> int -> int -> unit
	//Array.blit source sourceIndex target targetIndex count
	template<typename T>
	void blit(vector<T> const& source, size_t sourceIndex, vector<T>& target,
		size_t targetIndex, size_t count)
	{
		auto first1 = source.begin() + sourceIndex, last1 = first1 + count;
		auto first2 = target.begin() + targetIndex;
		std::copy(first1, last1, first2);
	}
	//Array.choose : ('T -> 'U option) -> 'T [] -> 'U []
	//Array.choose chooser array
	template<typename Fn, typename T>
	auto choose(Fn chooser, vector<T> const& array1) -> vector<typename remove_reference<decltype(*chooser(T()))>::type>
	{
		vector<typename remove_reference<decltype(*chooser(T()))>::type> vector_result;
		for_each(array1.begin(), array1.end(), 
			[&](T elem){
				if(auto v = chooser(elem))
					vector_result.push_back(*v);
			}
		);
		return vector_result;
	}
	//Array.collect : ('T -> 'U []) -> 'T [] -> 'U []
	//Array.collect mapping array
	template<typename Fn, typename T>
	auto collect(Fn mapping, vector<T> array1) -> decltype(mapping(T()))
	{
		decltype(mapping(T())) vector_result;
		for_each(array1.begin(), array1.end(),
			[&](T elem){
				auto vectorU = mapping(elem);
				vector_result.insert(vector_result.end(), vectorU.begin(), vectorU.end());
			}
		);
		return vector_result;
	}
	//Array.copy : 'T [] -> 'T []
	//Array.copy array
	template<typename T>
	vector<T> copy(vector<T> array1)
	{
		return array1;
	}
	//Array.create : int -> 'T -> 'T []
	//Array.create count value
	template<typename T>
	vector<T> create(size_t count, T value)
	{
		return vector<T>(count, value);
	}
	//Array.empty<'T> :  'T vector
	//Array.empty
	template<typename T>
	vector<T> empty()
	{
		return vector<T>();
	}
	//Array.exists : ('T -> bool) -> 'T [] -> bool
	//Array.exists predicate array
	template<typename Fn, typename T>
	bool exists(Fn predicate, vector<T> const& array1)
	{
		return any_of(array1.begin(), array1.end(), predicate);
	}
	//Array.exists2 : ('T1 -> 'T2 -> bool) -> 'T1 [] -> 'T2 [] -> bool
	//Array.exists2 predicate array1 array2
	template<typename Fn2, typename T1, typename T2>
	bool exists2(Fn2 predicate, vector<T1> const& array1, vector<T2> const& array2)
	{
		return !equal(array1.begin(), array1.end(), array2.begin(), 
			[&](T1 elem1, T2 elem2){return !predicate(elem1, elem2);});
	}
	//Array.filter : ('T -> bool) -> 'T [] -> 'T []
	//Array.filter predicate array
	template<typename Fn, typename T>
	vector<T> filter(Fn predicate, vector<T> array1)
	{
		array1.erase(std:::remove_if(array1.begin(), array1.end(),
			[&](T elem){return !predicate(elem);}), array1.end());
		return array1;
	}
	//Array.fill : 'T [] -> int -> int -> 'T -> unit
	//Array.fill target targetIndex count value
	template<typename T>
	void fill(vector<T>& target, size_t targetIndex, size_t count, T value)
	{
		auto first = target.begin() + targetIndex, last = first + count;
		std::fill(first, last, value);
	}
	//Array.find : ('T -> bool) -> 'T [] -> 'T
	//Array.find predicate array
	template<typename Fn, typename T>
	T find(Fn predicate, vector<T> const& array1)
	{
		auto i = find_if(array1.begin(), array1.end(), predicate);
		if(i == array1.end())
			throw runtime_error("");
		return *i;
	}
	//Array.findIndex : ('T -> bool) -> 'T [] -> int
	//Array.findIndex predicate array
	template<typename Fn, typename T>
	size_t findIndex(Fn predicate, vector<T> const& array1)
	{
		size_t index = 0;
		auto i = find_if(array1.begin(), array1.end(), 
			[&](T elem){return predicate(elem) ? true : (++index, false);});
		if(i == array1.end())
			throw runtime_error("");
		return index;
	}
	//Array.fold : ('State -> 'T -> 'State) -> 'State -> 'T [] -> 'State
	//Array.fold folder state array
	template<typename Fn2, typename State, typename T>
	State fold(Fn2 folder, State state, vector<T> const& array1)
	{
		return accumulate(array1.begin(), array1.end(), state, folder);
	}
	//Array.fold2 : ('State -> 'T1 -> 'T2 -> 'State) -> 'State -> 'T1 [] -> 'T2 [] -> 'State
	//Array.fold2 folder state array1 array2
	template<typename Fn3, typename State, typename T1, typename T2>
	State fold2(Fn3 folder, State state, vector<T1> const& array1, vector<T2> const& array2)
	{
		auto first2 = array2.begin();
		return accumulate(array1.begin(), array1.end(), state,
			[&](State state, T1 elem1) -> State {return folder(state, elem1, *first2++);});
	}
	//Array.foldBack : ('T -> 'State -> 'State) -> 'T [] -> 'State -> 'State
	//Array.foldBack folder array state
	template<typename Fn2, typename T, typename State>
	State foldBack(Fn2 folder, vector<T> const& array1, State state)
	{
		return accumulate(array1.rbegin(), array1.rend(), state, 
			[&](State state, T elem) -> State {return folder(elem, state);});
	}
	//Array.foldBack2 : ('T1 -> 'T2 -> 'State -> 'State) -> 'T1 vector -> 'T2 vector -> 'State -> 'State
	//Array.foldBack2 folder array1 array2 state
	template<typename Fn3, typename T1, typename T2, typename State>
	State foldBack2(Fn3 folder, vector<T1> const& array1, vector<T2> const& array2, State state)
	{
		auto first2 = array2.rbegin();
		return accumulate(array1.rbegin(), array1.rend(), state, 
			[&](State state, T1 elem1) -> State {return folder(elem1, *first2++, state);});
	}
	//Array.forall : ('T -> bool) -> 'T vector -> bool
	//Array.forall predicate vector	
	template<typename Fn, typename T>
	bool forall(Fn predicate, vector<T> const& array1)
	{
		return all_of(array1.begin(), array1.end(), predicate);
	}
	//Array.forall2 : ('T1 -> 'T2 -> bool) -> 'T1 vector -> 'T2 vector -> bool
	//Array.forall2 predicate array1 array2
	template<typename Fn2, typename T1, typename T2>
	bool forall2(Fn2 predicate, vector<T1> const& array1, vector<T2> const& array2)
	{
		return equal(array1.begin(), array1.end(), array2.begin(), predicate);
	}
	//Array.get : 'T [] -> int -> 'T
	//Array.get array index
	template<typename T>
	T get(vector<T> const& array1, size_t index)
	{
		return array1[index];
	}
	//Array.init : int -> (int -> 'T) -> 'T []
	//Array.init count initializer
	template<typename Fn>
	auto init(size_t count, Fn initializer) -> vector<decltype(initializer(size_t()))>
	{
		vector<decltype(initializer(size_t()))> vector_result(count);
		size_t index = 0;
		generate(vector_result.begin(), vector_result.end(), [&]{return initializer(index++);});
		return vector_result;
	}
	//Array.isEmpty : 'T [] -> bool
	//Array.isEmpty array
	template<typename T>
	bool isEmpty(vector<T> const& array1)
	{
		return array1.empty();
	}
	//Array.iter : ('T -> unit) -> 'T [] -> unit
	//Array.iter action array
	template<typename Fn, typename T>
	void iter(Fn action, vector<T> const& array1)
	{
		for_each(array1.begin(), array1.end(), action);
	}
	//Array.iter2 : ('T1 -> 'T2 -> unit) -> 'T1 [] -> 'T2 [] -> unit
	//Array.iter2 action array1 array2	
	template<typename Fn2, typename T1, typename T2>
	void iter2(Fn2 action, vector<T1> const& array1, vector<T2> const& array2)
	{
		auto first2 = array2.begin();
		for_each(array1.begin(), array1.end(), [&](T elem){action(elem, *first2++);});
	}
	//Array.iteri : (int -> 'T -> unit) -> 'T [] -> unit
	//Array.iteri action array
	template<typename Fn2, typename T>
	void iteri(Fn2 action, vector<T> const& array1)
	{
		size_t index = 0;
		for_each(array1.begin(), array1.end(), [&](T elem){action(index++, elem);});
	}
	//Array.iteri2 : (int -> 'T1 -> 'T2 -> unit) -> 'T1 [] -> 'T2 [] -> unit
	//Array.iteri2 action array1 array2
	template<typename Fn3, typename T1, typename T2>
	void iteri2(Fn3 action, vector<T1> const& array1, vector<T2> const& array2)
	{
		auto first2 = array2.begin();
		size_t index = 0;
		for_each(array1.begin(), array1.end(), [&](T elem){action(index++, elem, *first2++);});
	}
	//Array.length : 'T [] -> int
	//Array.length array
	template<typename T>
	size_t length(vector<T> const& array1)
	{
		return array1.size();
	}
	//Array.map : ('T -> 'U) -> 'T [] -> 'U []
	//Array.map mapping array
	template<typename Fn, typename T>
	auto map(Fn mapping, vector<T> const& array1) -> vector<decltype(mapping(T()))>
	{
		vector<decltype(mapping(T()))> vector_result(array1.size());
		transform(array1.begin(), array1.end(), vector_result.begin(), mapping);
		return vector_result;
	}
	//Array.map2 : ('T1 -> 'T2 -> 'U) -> 'T1 [] -> 'T2 [] -> 'U []
	//Array.map2 mapping array1 array2
	template<typename Fn2, typename T1, typename T2>
	auto map2(Fn2 mapping, vector<T1> const& array1, vector<T2> const& array2) -> vector<decltype(mapping(T1(), T2()))>
	{
		vector<decltype(mapping(T1(), T2()))> vector_result(array1.size());
		transform(array1.begin(), array1.end(), array2.begin(), vector_result.begin(), mapping);
		return vector_result;
	}
	//Array.map3 : ('T1 -> 'T2 -> 'T3 -> 'U) -> 'T1 [] -> 'T2 [] -> 'T3 [] -> 'U []
	//Array.map3 mapping array1 array2 array3
	template<typename Fn2, typename T1, typename T2, typename T3>
	auto map3(Fn2 mapping, vector<T1> const& array1, vector<T2> const& array2, vector<T3> const& array3)
		-> vector<decltype(mapping(T1(), T2(), T3()))>
	{
		vector<decltype(mapping(T1(), T2(), T3()))> vector_result(array1.size());
		auto first3 = array3.begin();
		transform(array1.begin(), array1.end(), array2.begin(), vector_result.begin(), 
			[&](T1 elem1, T2 elem2){return mapping(elem1, elem2, *first3++);});
		return vector_result;
	}
	//Array.mapi : (int -> 'T -> 'U) -> 'T [] -> 'U []
	//Array.mapi mapping array
	template<typename Fn2, typename T>
	auto mapi(Fn2 mapping, vector<T> const& array1) -> vector<decltype(mapping(size_t(), T()))>
	{
		vector<decltype(mapping(size_t(), T()))> vector_result(array1.size());
		size_t index = 0;
		transform(array1.begin(), array1.end(), vector_result.begin(), 
			[&](T elem){return mapping(index++, elem);});
		return vector_result;
	}
	//Array.mapi2 : (int -> 'T1 -> 'T2 -> 'U) -> 'T1 [] -> 'T2 [] -> 'U []
	//Array.mapi2 mapping array1 array2
	template<typename Fn3, typename T1, typename T2>
	auto mapi2(Fn3 mapping, vector<T1> const& array1, vector<T2> const& array2)
		-> vector<decltype(size_t(), mapping(T1(), T2()))>
	{
		vector<decltype(mapping(size_t(), T1(), T2()))> vector_result(array1.size());
		size_t index = 0;
		transform(array1.begin(), array1.end(), array2.begin(), vector_result.begin(), 
			[&](T1 elem1, T2 elem2){return mapping(index++, elem1, elem2);});
		return vector_result;
	}
	//Array.max : 'T [] -> 'T (requires comparison)
	//Array.max array
	template<typename T>
	T max(vector<T> array1)
	{
		return *max_element(array1.begin(), array1.end());
	}
	//Array.maxBy : ('T -> 'U) -> 'T [] -> 'T (requires comparison)
	//Array.maxBy projection vector
	template<typename Fn, typename T>
	T maxBy(Fn projection, vector<T> array1)
	{
		return *max_element(array1.begin(), array1.end(),
			[&](T elem1, T elem2){return projection(elem1) < projection(elem2);});
	}
	//Array.min : 'T [] -> 'T (requires comparison)
	//Array.min array
	template<typename T>
	T min(vector<T> array1)
	{
		return *min_element(array1.begin(), array1.end());
	}
	//Array.minBy : ('T -> 'U) -> 'T [] -> 'T (requires comparison)
	//Array.minBy projection array
	template<typename Fn, typename T>
	T minBy(Fn projection, vector<T> array1)
	{
		return *min_element(array1.begin(), array1.end(),
			[&](T elem1, T elem2){return projection(elem1) < projection(elem2);});
	}
	//Array.partition : ('T -> bool) -> 'T [] -> 'T [] * 'T []
	//Array.partition predicate array
	template<typename Fn, typename T>
	tuple<vector<T>, vector<T>> partition(Fn predicate, vector<T> const& array1)
	{
		vector<T> array2, vector3;
		for_each(array1.begin(), array1.end(), 
			[&](T elem){predicate(elem) ? array2.push_back(elem) : vector3.push_back(elem);}
		);
		return make_tuple(array2, vector3);
	}
	//Array.permute : (int -> int) -> 'T [] -> 'T []
	//Array.permute indexMap array
	template<typename Fn, typename T>
	vector<T> permute(Fn indexMap, vector<T> const& array1)
	{
		vector<T> vector_result(array1.size());
		size_t index = 0;
		for_each(array1.begin(), array1.end(), [&](T elem){
			*boost::next(vector_result.begin(), indexMap(index++)) = elem;
		});
		return vector_result;
	}
	//Array.pick : ('T -> 'U option) -> 'T [] -> 'U
	//Array.pick chooser array
	template<typename Fn, typename T>
	auto pick(Fn chooser, vector<T> const& array1) -> typename remove_reference<decltype(*chooser(T()))>::type
	{
		decltype(chooser(T())) opt;
		auto i = find_if(array1.begin(), array1.end(), 
			[&](T elem) -> bool{return opt = chooser(elem);});
		if(i == array1.end())
			throw runtime_error("");
		return *opt;
	}
	//Array.reduce : ('T -> 'T -> 'T) -> 'T [] -> 'T
	//Array.reduce reduction array
	template<typename Fn2, typename T>
	T reduce(Fn2 reduction, vector<T> const& array1)
	{
		if(array1.empty())
			throw runtime_error("");
		return accumulate(++array1.begin(), array1.end(), array1.front(), reduction);
	}
	//Array.reduceBack : ('T -> 'T -> 'T) -> 'T [] -> 'T
	//Array.reduceBack reduction array
	template<typename Fn2, typename T>
	T reduceBack(Fn2 reduction, vector<T> const& array1)
	{
		if(array1.empty())
			throw runtime_error("");
		return accumulate(++array1.rbegin(), array1.rend(), array1.back(), 
			[&](T state, T elem) -> T {return reduction(elem, state);});
	}
	//Array.rev : 'T [] -> 'T []
	//Array.rev array
	template<typename T>
	vector<T> rev(vector<T> array1)
	{
		return array1.reverse(), array1;
	}
	//Array.scan : ('State -> 'T -> 'State) -> 'State -> 'T [] -> 'State []
	//Array.scan folder state array
	template<typename Fn2, typename State, typename T>
	vector<State> scan(Fn2 folder, State state, vector<T> const& array1)
	{
		vector<State> vector_result(array1.size() + 1);
		auto first2 = vector_result.begin();
		*first2++ = state;
		for_each(array1.begin(), array1.end(), 
			[&](T elem){*first2++ = state = folder(state, elem);});
		return vector_result;
	}
	//Array.scanBack : ('T -> 'State -> 'State) -> 'T [] -> 'State -> 'State []
	//Array.scanBack folder array state
	template<typename Fn2, typename T, typename State>
	vector<State> scanBack(Fn2 folder, vector<T> const& array1, State state)
	{
		vector<State> vector_result(array1.size() + 1);
		auto first2 = vector_result.rbegin();
		*first2++ = state;
		for_each(array1.rbegin(), array1.rend(), 
			[&](T elem){*first2++ = state = folder(elem, state);});
		return vector_result;
	}
	//Array.set : 'T [] -> int -> 'T -> unit
	//Array.set array index value
	template<typename T>
	void set(vector<T>& array1, size_t index, T value)
	{
		array1[index] = value;
	}
	//Array.sortWith : ('T -> 'T -> int) -> 'T [] -> 'T []
	//Array.sortWith comparer array
	template<typename Fn2, typename T>
	vector<T> sortWith(Fn2 comparer, vector<T> array1)
	{
		std::sort(array1.begin(), array1.end(), [&](T elem1, T elem2){return comparer(elem1, elem2) < 0;});
		return array1;
	}
	//Array.sortBy : ('T -> 'Key) -> 'T [] -> 'T [] (requires comparison)
	//Array.sortBy projection array
	template<typename Fn, typename T>
	vector<T> sortBy(Fn projection, vector<T> array1)
	{
		std::sort(array1.begin(), array1.end(), [&](T elem1, T elem2){return projection(elem1) < projection(elem2);});
		return array1;
	}
	//Array.sort : 'T [] -> 'T [] (requires comparison)
	//Array.sort array
	template<typename T>
	vector<T> sort(vector<T> array1)
	{
		std::sort(array1.begin(), array1.end());
		return array1;
	}
	//Array.sortInPlaceWith : ('T -> 'T -> int) -> 'T [] -> unit
	//Array.sortInPlaceWith comparer array
	template<typename Fn2, typename T>
	void sortInPlaceWith(Fn2 comparer, vector<T>& array1)
	{
		std::sort(array1.begin(), array1.end(), [&](T elem1, T elem2){return comparer(elem1, elem2) < 0;});
	}
	//Array.sortInPlaceBy : ('T -> 'Key) -> 'T [] -> unit (requires comparison)
	//Array.sortInPlaceBy projection array
	template<typename Fn, typename T>
	void sortInPlaceBy(Fn projection, vector<T>& array1)
	{
		std::sort(array1.begin(), array1.end(), [&](T elem1, T elem2){return projection(elem1) < projection(elem2);});
	}
	//Array.sortInPlace : 'T [] -> unit (requires comparison)
	//Array.sortInPlace array
	template<typename T>
	void sortInPlace(vector<T>& array1)
	{
		std::sort(array1.begin(), array1.end());
	}
	//Array.sub : 'T [] -> int -> int -> 'T []
	//Array.sub array startIndex count
	template<typename T>
	vector<T> sub(vector<T> const& array1, size_t startIndex, size_t count)
	{
		auto first = array1.begin(), last = first + count;
		return vector<T>(first, last);
	}
	//Array.sum : ^T [] -> ^T (requires ^T with static member (+) and ^T with static member Zero)
	//Array.sum array
	template<typename T>
	T sum(vector<T> const& array1)
	{
		return accumulate(array1.begin(), array1.end(), T());
	}
	//Array.sumBy : ('T -> ^U) -> 'T [] -> ^U (requires ^U with static member (+) and ^U with static member Zero)
	//Array.sumBy projection array
	template<typename Fn, typename T>
	auto sumBy(Fn projection, vector<T> const& array1) -> decltype(projection(T()))
	{
		typedef decltype(projection(T())) U;
		return accumulate(array1.begin(), array1.end(), U(),
			[&](U acc, T elem){return acc + projection(elem);});
	}
	//Array.tail : 'T [] -> 'T []
	//Array.tail array
	template<typename T>
	vector<T> tail(vector<T> array1)
	{
		return array1.erase(array1.begin()), array1;
	}
	//Array.tryPick : ('T -> 'U option) -> 'T [] -> 'U option
	//Array.tryPick chooser array
	template<typename Fn, typename T>
	auto tryPick(Fn chooser, vector<T> const& array1) -> decltype(chooser(T()))
	{
		decltype(chooser(T())) opt;
		find_if(array1.begin(), array1.end(), 
			[&](T elem) -> bool{return opt = chooser(elem);});
		return opt;
	}
	//Array.tryFind : ('T -> bool) -> 'T [] -> 'T option
	//Array.tryFind predicate array
	template<typename Fn, typename T>
	optional<T> tryFind(Fn predicate, vector<T> const& array1)
	{
		auto i = find_if(array1.begin(), array1.end(), predicate);
		return i == array1.end() ? optional<T>() : optional<T>(*i);
	}
	//Array.tryFindIndex : ('T -> bool) -> 'T [] -> int option
	//Array.tryFindIndex predicate vector
	template<typename Fn, typename T>
	optional<size_t> tryFindIndex(Fn predicate, vector<T> const& array1)
	{
		size_t index = 0;
		auto i = find_if(array1.begin(), array1.end(), 
			[&](T elem){return predicate(elem) ? true : (++index, false);});
		return i == array1.end() ? optional<size_t>() : optional<size_t>(index);
	}
	//Array.zip : 'T1 [] -> 'T2 [] -> 'T1 * 'T2 vector
	//Array.zip array1 array2
	template<typename T1, typename T2>
	vector<tuple<T1, T2>> zip(vector<T1> const& array1, vector<T2> const& array2)
	{
		vector<tuple<T1, T2>> vector_result(array1.size());
		transform(array1.begin(), array1.end(), array2.begin(), vector_result.begin(), 
			[&](T1 elem1, T2 elem2){return make_tuple(elem1, elem2);});
		return vector_result;
	}
	//Array.zip3 : 'T1 [] -> 'T2 [] -> 'T3 [] -> 'T1 * 'T2 * 'T3 []
	//Array.zip3 array1 array2 array3
	template<typename T1, typename T2, typename T3>
	vector<tuple<T1, T2, T3>> zip3(vector<T1> const& array1, vector<T2> const& array2, vector<T3> const& array3)
	{
		vector<boost::tuple<T1, T2, T3>> vector_result(array1.size());
		auto first3 = array3.begin();
		transform(array1.begin(), array1.end(), array2.begin(), vector_result.begin(), 
			[&](T1 elem1, T2 elem2){return make_tuple(elem1, elem2, *first3++);});
		return vector_result;
	}
	//Array.unzip : 'T1 * 'T2 [] -> 'T1 [] * 'T2 []
	//Array.unzip array
	template<typename T1, typename T2>
	tuple<vector<T1>, vector<T2>> unzip(vector<tuple<T1, T2>> const& array1)
	{
		vector<T1> array2;
		vector<T2> vector3;
		for_each(array1.begin(), array1.end(), [&](tuple<T1, T2> const& t){
			array2.push_back(get<0>(t));
			vector3.push_back(get<1>(t));
		});
		return make_tuple(array2, vector3);
	}
	//Array.unzip3 : 'T1 * 'T2 * 'T3 [] -> 'T1 [] * 'T2 [] * 'T3 []
	//Array.unzip3 array
	template<typename T1, typename T2, typename T3>
	tuple<vector<T1>, vector<T2>, vector<T3>> unzip3(vector<tuple<T1, T2, T3>> const& array1)
	{
		vector<T1> array2;
		vector<T2> vector3;
		vector<T3> vector4;
		for_each(array1.begin(), array1.end(), [&](tuple<T1, T2, T3> const& t){
			array2.push_back(get<0>(t));
			vector3.push_back(get<1>(t));
			vector4.push_back(get<2>(t));
		});
		return make_tuple(array2, vector3, vector4);
	}
}

namespace std
{
	namespace karma = boost::spirit::karma;
	template<typename T>
	ostream& operator<<(ostream& os, vector<T> const& array1)
	{
		return os << karma::format("[|" << -(karma::auto_ % "; ") << "|]", array1);
	}
	template<typename T1, typename T2>
	ostream& operator<<(ostream& os, boost::tuple<T1, T2> const& array1)
	{
		return os << karma::format(karma::auto_, array1);
	}
	template<typename T1, typename T2, typename T3>
	ostream& operator<<(ostream& os, boost::tuple<T1, T2, T3> const& array1)
	{
		return os << karma::format(karma::auto_, array1);
	}
}

namespace boost { namespace spirit { namespace traits
{
	template <typename T>
	struct create_generator<std::vector<T>>
	{
		typedef proto::result_of::deep_copy<
			BOOST_TYPEOF("[|" << -(karma::auto_ % "; ") << "|]")
		>::type type;

		static type call()
		{
			return proto::deep_copy(
				"[|" << -(karma::auto_ % "; ") << "|]");
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
