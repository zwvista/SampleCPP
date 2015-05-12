// fsharp2cpp.cpp : Defines the entry point for the console application.
//

#include "fsharp_list.h"
#include <boost/assign.hpp>
#include <boost/format.hpp>

using namespace std;
using namespace boost::assign;
using boost::tuple;
using boost::optional;
using boost::format;
using boost::io::group;

void test_exists()
{
	cout << endl << __FUNCTION__ << endl;
	//let containsNumber number list = List.exists (fun elem -> elem = number) list
	//let list0to3 = [0 .. 3]
	//printfn "For list %A, contains zero is %b" list0to3 (containsNumber 0 list0to3)
	list<int> list0to3 = list_of(0)(1)(2)(3);
	auto containsNumber = [](int number, list<int> const& l){
		return fsharp_list::exists([number](int elem){return elem == number;}, l);
	};
	cout << format("For list %1%, contains zero is %2%\n") % list0to3 % group(boolalpha, containsNumber(0, list0to3));
	//For list [0; 1; 2; 3], contains zero is true

	//let isEqualElement list1 list2 = List.exists2 (fun elem1 elem2 -> elem1 = elem2) list1 list2
	//let list1to5 = [ 1 .. 5 ]
	//let list5to1 = [ 5 .. -1 .. 1 ]
	//if (isEqualElement list1to5 list5to1) then
	//	printfn "Lists %A and %A have at least one equal element at the same position." list1to5 list5to1
	//else
	//	printfn "Lists %A and %A do not have an equal element at the same position." list1to5 list5to1
	list<int> list1to5 = list_of(1)(2)(3)(4)(5);
	list<int> list5to1 = list_of(5)(4)(3)(2)(1);
	auto isEqualElement = [](list<int> const& l1, list<int> const& l2){
		return fsharp_list::exists2([](int elem1, int elem2){return elem1 == elem2;}, l1, l2);
	};
	if(isEqualElement(list1to5, list5to1))
		cout << format("Lists %1% and %2% have at least one equal element at the same position.\n") % list1to5 % list5to1;
	else
		cout << format("Lists %1% and %2% do not have an equal element at the same position.\n") % list1to5 % list5to1;
	//Lists [1; 2; 3; 4; 5] and [5; 4; 3; 2; 1] have at least one equal element at the same position.
}

void test_forall()
{
	cout << endl << __FUNCTION__ << endl;
	//let isAllZeroes list = List.forall (fun elem -> elem = 0.0) list
	//printfn "%b" (isAllZeroes [0.0; 0.0])
	//printfn "%b" (isAllZeroes [0.0; 1.0])
	auto isAllZeroes = [](list<double> const& l){
		return fsharp_list::forall([](double elem){return elem == 0.0;}, l);
	};
	cout << isAllZeroes(list<double>(list_of(0.0)(0.0))) << endl;
	cout << isAllZeroes(list<double>(list_of(0.0)(1.0))) << endl;
	//true
	//false

	//let listEqual list1 list2 = List.forall2 (fun elem1 elem2 -> elem1 = elem2) list1 list2
	//printfn "%b" (listEqual [0; 1; 2] [0; 1; 2])
	//printfn "%b" (listEqual [0; 0; 0] [0; 1; 0])
	auto listEqual = [](list<int> const& l1, list<int> const& l2){
		return fsharp_list::forall2([](int elem1, int elem2){return elem1 == elem2;}, l1, l2);
	};
	cout << listEqual(list<int>(list_of(0)(1)(2)), list<int>(list_of(0)(1)(2))) << endl;
	cout << listEqual(list<int>(list_of(0)(0)(0)), list<int>(list_of(0)(1)(0))) << endl;
	//true
	//false
}

void test_sort()
{
	cout << endl << __FUNCTION__ << endl;
	//let list1 = [1; 4; 8; -2; 5]
	//let sortedList1 = List.sort list1
	//printfn "%A" sortedList1
	list<int> list1 = list_of(1)(4)(8)(-2)(5);
	auto sortedList1 = fsharp_list::sort(list1);
	cout << sortedList1 << endl;
	//[-2; 1; 4; 5; 8]

	//let sortedList2 = List.sortBy (fun elem -> abs elem) list1
	//printfn "%A" sortedList2
	auto sortedList2 = fsharp_list::sortBy([](int elem){return abs(elem);}, list1);
	cout << sortedList2 << endl;
	//[1; -2; 4; 5; 8]

	//let compareElem elem1 elem2 =
	//    if abs elem1 < abs elem2 then -1 else
	//    if abs elem1 = abs elem2 then 0 else
	//    1
	//let sortedList3 = List.sortWith compareElem list1
	//printfn "%A" sortedList3
	auto compareElem = [](int elem1, int elem2){
		return abs(elem1) < abs(elem2) ? -1 : abs(elem1) == abs(elem2) ? 0 : 1;
	};
	auto sortedList3 = fsharp_list::sortWith(compareElem, list1);
	cout << sortedList3 << endl;
	//[1; -2; 4; 5; 8]

	//let min1 = List.min list1
	//let max1 = List.max list1
	//let min2 = List.minBy (fun elem -> abs elem) list1
	//let max2 = List.maxBy (fun elem -> abs elem) list1
	//printfn "%d %d %d %d" min1 max1 min2 max2
	auto min1 = fsharp_list::min(list1);
	auto max1 = fsharp_list::max(list1);
	auto min2 = fsharp_list::minBy([](int elem){return abs(elem);}, list1);
	auto max2 = fsharp_list::maxBy([](int elem){return abs(elem);}, list1);
	cout << format("%1% %2% %3% %4%\n") % min1 % max1 % min2 % max2;
	//-2 8 1 8
}

void test_find()
{
	cout << endl << __FUNCTION__ << endl;
	//let isDivisibleBy number elem = elem % number = 0
	//let result = List.find (isDivisibleBy 5) [ 1 .. 100 ]
	//let result = List.findIndex (isDivisibleBy 5) [ 1 .. 100 ]
	//printfn "%d " result
	list<int> l(100);
	iota(l.begin(), l.end(), 1);
	auto isDivisibleBy = [](int number, int elem){return elem % number == 0;};
	cout << fsharp_list::find([isDivisibleBy](int elem){return isDivisibleBy(5, elem);}, l) << endl;
	cout << fsharp_list::findIndex([isDivisibleBy](int elem){return isDivisibleBy(5, elem);}, l) << endl;
	//5
	//4

	//let list1 = [1; 3; 7; 9; 11; 13; 15; 19; 22; 29; 36]
	//let isEven x = x % 2 = 0
	//match List.tryFind isEven list1 with
	//    | Some value -> printfn "The first even value is %d." value
	//    | None -> printfn "There is no even value in the list."
	//match List.tryFindIndex isEven list1 with
	//    | Some value -> printfn "The first even value is at position %d." value
	//    | None -> printfn "There is no even value in the list."

	list<int> list1 = list_of(1)(3)(7)(9)(11)(13)(15)(19)(22)(29)(36);
	auto isEven = [](int x){return x % 2 == 0;};
	if(auto result = fsharp_list::tryFind(isEven, list1))
		cout << format("The first even value is %1%.\n") % *result;
	else
		cout << "There is no even value in the list.\n";
	if(auto result = fsharp_list::tryFindIndex(isEven, list1))
		cout << format("The first even value is at position %1%.\n") % *result;
	else
		cout << "There is no even value in the list.\n";
	//The first even value is 22.
	//The first even value is at position 8.

	//let valuesList = [ ("a", 1); ("b", 2); ("c", 3) ]
	//let result = List.pick (fun elem -> if (snd elem = 2) then Some(fst elem) else None) valuesList
	//printfn "%A" result
	//let result2 = List.tryPick (fun elem -> if (snd elem = 2) then Some(fst elem) else None) valuesList
	//printfn "%A" result2
	list<pair<string, int>> valuesList = list_of<pair<string, int>>("a", 1)("b", 2)("c", 3);
	auto result = fsharp_list::pick(
		[](pair<string, int> const& elem){
			return elem.second == 2 ? optional<string>(elem.first) : optional<string>();
	}, valuesList);
	cout << result << endl;
	auto result2 = fsharp_list::tryPick(
		[](pair<string, int> const& elem){
			return elem.second == 2 ? optional<string>(elem.first) : optional<string>();
	}, valuesList);
	cout << *result2 << endl;
	//"b"
	//"b"
}

void test_other()
{
	cout << endl << __FUNCTION__ << endl;
	//let sum1 = List.sum [1 .. 10]
	//let sum2 = List.sumBy (fun elem -> elem*elem) [1 .. 10]
	//printfn "%d" sum1
	//printfn "%d" sum2
	//let avg1 = List.average [0.0; 1.0; 1.0; 2.0]
	//let avg2 = List.averageBy (fun elem -> float elem) [1 .. 10]
	//printfn "%f" avg1
	//printfn "%f" avg2

	list<int> l(10);
	iota(l.begin(), l.end(), 1);
	auto sum1 = fsharp_list::sum(l);
	auto sum2 = fsharp_list::sumBy([](int elem){return elem * elem;}, l);
	cout << sum1 << endl;
	cout << sum2 << endl;
	auto avg1 = fsharp_list::average(list<double>(list_of(0.0)(1.0)(1.0)(2.0)));
	auto avg2 = fsharp_list::averageBy([](int elem) -> double {return elem;}, l);
	cout << avg1 << endl;
	cout << avg2 << endl;
	//55
	//385
	//1.0
	//5.5
	//let list1 = [1; 2; 3; 4; 5; 6]
	//let list_tuple = List.partition (fun x -> x % 2 = 0) list1
	//printfn "%A" list_tuple
	list<int> list1 = list_of(1)(2)(3)(4)(5)(6);
	auto list_tuple = fsharp_list::partition([](int x){return x % 2 == 0;}, list1);
	cout << list_tuple << endl;
	//([2; 4; 6], [1; 3; 5])

	//let listPermuted = List.permute (fun x -> 5 - x) list1
	//printfn "%A" listPermuted
	auto listPermuted = fsharp_list::permute([](size_t x)->size_t{return 5 - x;}, list1);
	cout << listPermuted << endl;
	//[6; 5; 4; 3; 2; 1]

	//printfn "%A" (List.append list1 list1)
	//printfn "%A" List.empty
	//printfn "%A" (List.head list1)
	//printfn "%A" (List.init 3 (fun i -> i * 2))
	//printfn "%b" (List.isEmpty list1)
	//printfn "%b" (List.isEmpty List.empty)
	//printfn "%d" (List.length list1)
	//printfn "%d" (List.nth list1 3)
	//printfn "%A" (List.replicate 3 100)
	//printfn "%A" (List.rev list1)
	//printfn "%A" (List.tail list1)
	cout << fsharp_list::append(list1, list1) << endl;
	cout << fsharp_list::empty<int>() << endl;
	cout << fsharp_list::head(list1) << endl;
	cout << fsharp_list::init(3, [](size_t index){return index * 2;}) << endl;
	cout << fsharp_list::isEmpty(list1) << endl;
	cout << fsharp_list::isEmpty(fsharp_list::empty<int>()) << endl;
	cout << fsharp_list::length(list1) << endl;
	cout << fsharp_list::nth(list1, 3) << endl;
	cout << fsharp_list::replicate(3, 100) << endl;
	cout << fsharp_list::rev(list1) << endl;
	cout << fsharp_list::tail(list1) << endl;
	//[1; 2; 3; 4; 5; 6; 1; 2; 3; 4; 5; 6]
	//[]
	//1
	//[0; 2; 4]
	//false
	//true
	//6
	//4
	//[100; 100; 100]
	//[6; 5; 4; 3; 2; 1]
	//[2; 3; 4; 5; 6]
}

void test_zip()
{
	cout << endl << __FUNCTION__ << endl;
	//let list1 = [ 1; 2; 3 ]
	//let list2 = [ -1; -2; -3 ]
	//let listZip = List.zip list1 list2
	//printfn "%A" listZip
	//let list3 = [ 0; 0; 0]
	//let listZip3 = List.zip3 list1 list2 list3
	//printfn "%A" listZip3

	list<int> list1 = list_of(1)(2)(3);
	list<int> list2 = list_of(-1)(-2)(-3);
	auto listZip = fsharp_list::zip(list1, list2);
	cout << listZip << endl;
	list<int> list3 = list_of(0)(0)(0);
	auto listZip3 = fsharp_list::zip3(list1, list2, list3);
	cout << listZip3 << endl;
	//[(1, -1); (2, -2); (3, -3)]
	//[(1, -1, 0); (2, -2, 0); (3, -3, 0)]

	//let lists = List.unzip [(1,2); (3,4)]
	//printfn "%A" lists
	//printfn "%A %A" (fst lists) (snd lists)
	//let listsUnzip3 = List.unzip3 [(1,2,3); (4,5,6)]
	//printfn "%A" listsUnzip3
	auto lists = fsharp_list::unzip(list<tuple<int, int>>(tuple_list_of(1, 2)(3, 4)));
	cout << lists << endl;
	cout << format("%1% %2%\n") % get<0>(lists) % get<1>(lists);
	auto listsUnzip3 = fsharp_list::unzip3(list<tuple<int, int, int>>(tuple_list_of(1, 2, 3)(4, 5, 6)));
	cout << listsUnzip3 << endl;
	//([1; 3], [2; 4])
	//[1; 3] [2; 4]
	//([1; 4], [2; 5], [3; 6])
}

void test_iter()
{
	cout << endl << __FUNCTION__ << endl;
	//let lists = List.unzip [(1,2); (3,4)]
	//printfn "%A" lists
	//printfn "%A %A" (fst lists) (snd lists)
	//let listsUnzip3 = List.unzip3 [(1,2,3); (4,5,6)]
	//printfn "%A" listsUnzip3
	auto lists = fsharp_list::unzip(list<tuple<int, int>>(tuple_list_of(1, 2)(3, 4)));
	cout << lists << endl;
	cout << format("%1% %2%\n") % get<0>(lists) % get<1>(lists);
	auto listsUnzip3 = fsharp_list::unzip3(list<tuple<int, int, int>>(tuple_list_of(1, 2, 3)(4, 5, 6)));
	cout << listsUnzip3 << endl;
	//([1; 3], [2; 4])
	//[1; 3] [2; 4]
	//([1; 4], [2; 5], [3; 6])
}

void test_map()
{
	cout << endl << __FUNCTION__ << endl;
	//let list1 = [1; 2; 3]
	//let newList = List.map (fun x -> x + 1) list1
	//printfn "%A" newList
	//let list2 = [4; 5; 6]
	//let sumList = List.map2 (fun x y -> x + y) list1 list2
	//printfn "%A" sumList
	//let newList2 = List.map3 (fun x y z -> x + y + z) list1 list2 [2; 3; 4]
	//printfn "%A" newList2
	//let newListAddIndex = List.mapi (fun i x -> x + i) list1
	//printfn "%A" newListAddIndex
	//let listAddTimesIndex = List.mapi2 (fun i x y -> (x + y) * i) list1 list2
	//printfn "%A" listAddTimesIndex

	list<int> list1 = list_of(1)(2)(3);
	auto newList = fsharp_list::map([](int x){return x + 1;}, list1);
	cout << newList << endl;
	list<int> list2 = list_of(4)(5)(6);
	auto sumList = fsharp_list::map2([](int x, int y){return x + y;}, list1, list2);
	cout << sumList << endl;
	auto newList2 = fsharp_list::map3([](int x, int y, int z){return x + y + z;},
		list1, list2, list<int>(list_of(2)(3)(4)));
	cout << newList2 << endl;
	auto newListAddIndex = fsharp_list::mapi([](size_t i, int x){return x + i;}, list1);
	cout << newListAddIndex << endl;
	auto listAddTimesIndex = fsharp_list::mapi2([](size_t i, int x, int y) -> int{return (x + y) * i;}, list1, list2);
	cout << listAddTimesIndex << endl;
	//[2; 3; 4]
	//[5; 7; 9]
	//[7; 10; 13]
	//[1; 3; 5]
	//[0; 7; 18]

	//let collectList = List.collect (fun x -> [for i in 1..3 -> x * i]) list1
	//printfn "%A" collectList
	auto collectList = fsharp_list::collect(
		[](int x) -> list<int> {
			list<int> l;
			for(int i = 1; i <= 3; i++)
				l.push_back(x * i);
			return l;
		}, list1
	);
	cout << collectList << endl;
	//[1; 2; 3; 2; 4; 6; 3; 6; 9]

	//let evenOnlyList = List.filter (fun x -> x % 2 = 0) [1; 2; 3; 4; 5; 6]
	//printfn "%A" evenOnlyList
	auto evenOnlyList = fsharp_list::filter(
		[](int x){return x % 2 == 0;}, list<int>(list_of(1)(2)(3)(4)(5)(6))
	);
	cout << evenOnlyList << endl;
	//[2; 4; 6]

	//let listWords = [ "and"; "Rome"; "Bob"; "apple"; "zebra" ]
	//let isCapitalized (string1:string) =
	//    string1.Chars(0) >= 'A' && string1.Chars(0) <= 'Z'
	//let results = List.choose (fun elem -> if isCapitalized elem then Some(elem) else None) listWords
	//printfn "%A" results
	list<string> listWords = list_of<string>("and")("Rome")("Bob")("apple")("zebra");
	auto isCapitalized = [](string const& str){return str[0] >= 'A' && str[0] <= 'Z';};
	auto results = fsharp_list::choose(
		[&](string const& elem){
			return isCapitalized(elem) ? optional<string>(elem) : optional<string>();
		}, listWords
	);
	cout << results << endl;
	//[Rome; Bob]
}

void test_fold()
{
	cout << endl << __FUNCTION__ << endl;
	//let sumList list = List.fold (fun acc elem -> acc + elem) 0 list
	//printfn "Sum of the elements of list %A is %d." [ 1 .. 3 ] (sumList [ 1 .. 3 ])
	list<int> list1 = list_of(1)(2)(3);
	auto sumList = [](list<int> const& l){return fsharp_list::fold(
		[](int acc, int elem){return acc + elem;}, 0, l
	);};
	cout << format("Sum of the elements of list %1% is %2%.\n") % list1 % sumList(list1);
	//Sum of the elements of list [1; 2; 3] is 6.

	//let averageList list = (List.fold (fun acc elem -> acc + float elem) 0.0 list / float list.Length)
	//let stdDevList list =
	//    let avg = averageList list
	//    sqrt (List.fold (fun acc elem -> acc + (float elem - avg) ** 2.0 ) 0.0 list / float list.Length)
	//let testList listTest =
	//    printfn "List %A average: %f stddev: %f" listTest (averageList listTest) (stdDevList listTest)
	//testList [1; 1; 1]
	//testList [1; 2; 1]
	//testList [1; 2; 3]
	auto averageList = [](list<int> const& l){
		return fsharp_list::fold(
			[](double acc, int elem){return acc + elem;}, 0.0, l
		) / l.size();
	};
	auto stdDevList = [&](list<int> const& l) -> double {
		auto avg = averageList(l);
		return sqrt(fsharp_list::fold(
			[avg](double acc, int elem){return acc + (elem - avg) * (elem - avg);}, 0.0, l
		) / l.size());
	};
	auto testList = [&](list<int> const& l){
		cout << format("List %1% average: %2$.6f stddev: %3$.6f\n") % l % averageList(l) % stdDevList(l);
	};
	testList(list<int>(list_of(1)(1)(1)));
	testList(list<int>(list_of(1)(2)(1)));
	testList(list<int>(list_of(1)(2)(3)));
	//List [1; 1; 1] average: 1.000000 stddev: 0.000000
	//List [1; 2; 1] average: 1.333333 stddev: 0.471405
	//List [1; 2; 3] average: 2.000000 stddev: 0.816497

	//let printList list = List.fold (fun acc elem -> printfn "%A" elem) () list
	//printList [0.0; 1.0; 2.5; 5.1 ]
	auto printList = [](list<double> const& l){fsharp_list::fold(
		[](int acc, double elem){return cout << elem << endl, acc;}, 0, l
	);};
	printList(list<double>(list_of(0.0)(1.0)(2.5)(5.1)));
	//0
	//1
	//2.5
	//5.1

	//let reverseList list = List.fold (fun acc elem -> elem::acc) [] list
	//printfn "%A" (reverseList [1 .. 10])
	list<int> list2(10);
	iota(list2.begin(), list2.end(), 1);
	auto reverseList = [](list<int> const& l){
		return fsharp_list::fold(
			[](list<int> acc, int elem){return acc.push_front(elem), acc;}, list<int>(), l
		);};
	cout << reverseList(list2) << endl;
	//[10; 9; 8; 7; 6; 5; 4; 3; 2; 1]

	//let sumListBack list = List.foldBack (fun elem acc -> elem + acc) list 0
	//printfn "%d" (sumListBack [1; 2; 3])
	//let copyList list = List.foldBack (fun elem acc -> elem::acc) list []
	//printfn "%A" (copyList [1 .. 10])
	auto sumListBack = [](list<int> const& l){
		return fsharp_list::foldBack(
			[](int elem, int acc){return elem + acc;}, l, 0
		);};
	cout << sumListBack(list<int>(list_of(1)(2)(3))) << endl;
	auto copyList = [](list<int> const& l){
		return fsharp_list::foldBack(
			[](int elem, list<int> acc){return acc.push_front(elem), acc;}, l, list<int>()
		);};
	cout << copyList(list2) << endl;
	//[1; 2; 3; 4; 5; 6; 7; 8; 9; 10]

	//// Discriminated union type that encodes the transaction type.
	//type Transaction =
	//    | Deposit
	//    | Withdrawal
	//let transactionTypes = [Deposit; Deposit; Withdrawal]
	//let transactionAmounts = [100.00; 1000.00; 95.00 ]
	//let initialBalance = 200.00
	//// Use fold2 to perform a calculation on the list to update the account balance.
	//let endingBalance = List.fold2 (fun acc elem1 elem2 ->
	//                                match elem1 with
	//                                | Deposit -> acc + elem2
	//                                | Withdrawal -> acc - elem2)
	//                                initialBalance
	//                                transactionTypes
	//                                transactionAmounts
	//printfn "%f" endingBalance
	enum Transaction {Deposit, Withdrawal};
	list<Transaction> transactionTypes = list_of<Transaction>(Deposit)(Deposit)(Withdrawal);
	list<double> transactionAmounts = list_of(100.00)(1000.00)(95.00);
	auto initialBalance = 200.00;
	auto endingBalance = fsharp_list::fold2(
		[=](double acc, Transaction elem1, double elem2) -> double {
			return elem1 == Deposit ? acc + elem2 : acc - elem2;
		}, 
		initialBalance, transactionTypes, transactionAmounts
	);
	cout << format("%f\n") % endingBalance;
	//1205

	//type Transaction2 =
	//| Deposit
	//| Withdrawal
	//| Interest
	//
	//let transactionTypes2 = [Deposit; Deposit; Withdrawal; Interest]
	//let transactionAmounts2 = [100.00; 1000.00; 95.00; 0.05 / 12.0 ]
	//let initialBalance2 = 200.00
	//
	//// Because fold2 processes the lists by starting at the head element,
	//// the interest is calculated last, on the balance of 1205.00.
	//let endingBalance2 = List.fold2 (fun acc elem1 elem2 ->
	//                                match elem1 with
	//                                | Deposit -> acc + elem2
	//                                | Withdrawal -> acc - elem2
	//                                | Interest -> acc * (1.0 + elem2))
	//                                initialBalance2
	//                                transactionTypes2
	//                                transactionAmounts2
	//printfn "%f" endingBalance2
	//
	//// Because foldBack2 processes the lists by starting at end of the list,
	//// the interest is calculated first, on the balance of only 200.00.
	//let endingBalance3 = List.foldBack2 (fun elem1 elem2 acc ->
	//                                match elem1 with
	//                                | Deposit -> acc + elem2
	//                                | Withdrawal -> acc - elem2
	//                                | Interest -> acc * (1.0 + elem2))
	//                                transactionTypes2
	//                                transactionAmounts2
	//                                initialBalance2
	//printfn "%f" endingBalance3
	enum Transaction2 {Deposit2, Withdrawal2, Interest2};
	list<Transaction2> transactionTypes2 = list_of<Transaction2>(Deposit2)(Deposit2)(Withdrawal2)(Interest2);
	list<double> transactionAmounts2 = list_of(100.00)(1000.00)(95.00)(0.05 / 12.0);
	auto initialBalance2 = 200.00;
	auto endingBalance2 = fsharp_list::fold2(
		[=](double acc, Transaction2 elem1, double elem2) -> double {
			return elem1 == Deposit2 ? acc + elem2 :
				elem1 == Withdrawal2 ? acc - elem2 :
				acc * (1.0 + elem2);
		}, initialBalance2, transactionTypes2, transactionAmounts2
	);
	cout << format("%f\n") % endingBalance2;
	auto endingBalance3 = fsharp_list::foldBack2(
		[=](Transaction2 elem1, double elem2, double acc) -> double {
			return elem1 == Deposit2 ? acc + elem2 :
				elem1 == Withdrawal2 ? acc - elem2 :
				acc * (1.0 + elem2);
		}, transactionTypes2, transactionAmounts2, initialBalance2
	);
	cout << format("%f\n") % endingBalance3;
	//1210.020833
	//1205.833333

	//let list1 = [1; 2; 3]
	//let listState = List.scan (fun acc elem -> acc + elem) 0 list1
	//printfn "%A" listState
	//let listState2 = List.scanBack (fun elem acc -> elem + acc) list1 0
	//printfn "%A" listState2
	auto listState = fsharp_list::scan(
		[](int acc, int elem){return acc + elem;}, 0, list1
	);
	cout << listState << endl;
	auto listState2 = fsharp_list::scanBack(
		[](int elem, int acc){return elem + acc;}, list1, 0
	);
	cout << listState2 << endl;
	//[0; 1; 3; 6]
	//[6; 5; 3; 0]

	//let n1 = List.reduce (fun acc elem -> acc - elem) list1
	//let n2 = List.reduceBack (fun elem acc -> elem - acc) list1
	//printfn "%d %d" n1 n2
	auto n1 = fsharp_list::reduce([](int acc, int elem){return acc - elem;}, list1);
	auto n2 = fsharp_list::reduceBack([](int elem, int acc){return elem - acc;}, list1);
	cout << format("%1% %2%\n") % n1 % n2;
	//-4 2
 }

int main()
{
	cout << boolalpha << setprecision(6);
	test_exists();
	test_forall();
	test_sort();
	test_find();
	test_zip();
	test_iter();
	test_map();
	test_fold();
	test_other();
	return 0;
}


