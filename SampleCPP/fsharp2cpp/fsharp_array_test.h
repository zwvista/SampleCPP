// fsharp2cpp.cpp : Defines the entry point for the console application.
//

#include "fsharp_array.h"
#include <boost/assign.hpp>
#include <boost/format.hpp>

using namespace std;
using namespace boost::assign;
using boost::tuple;
using boost::optional;
using boost::format;

void test_exists()
{
	cout << endl << __FUNCTION__ << endl;
	//let allNegative = Array.exists (fun elem -> abs (elem) = elem) >> not
	//printfn "%A" (allNegative [| -1; -2; -3 |])
	//printfn "%A" (allNegative [| -10; -1; 5 |])
	//printfn "%A" (allNegative [| 0 |])
	//let haveEqualElement = Array.exists2 (fun elem1 elem2 -> elem1 = elem2)
	//printfn "%A" (haveEqualElement [| 1; 2; 3 |] [| 3; 2; 1|])
	auto allNegative = [](vector<int>& a){
		return !fsharp_array::exists([](int elem){return abs(elem) == elem;}, a);
	};
	cout << allNegative(vector<int>(list_of(-1)(-2)(-3)))) << endl;
	cout << allNegative(vector<int>(list_of(-10)(-1)(5)))) << endl;
	cout << allNegative(vector<int>(1, 0))) << endl;
	auto haveEqualElement = [](vector<int> const& a1, vector<int> const& a2){
		return fsharp_array::exists2([](int elem1, int elem2){return elem1 == elem2;}, a1, a2);
	};
	cout << haveEqualElement(vector<int>(list_of(1)(2)(3)), vector<int>(list_of(3)(2)(1)))) << endl;
	//true
	//false
	//false
	//true
}

void test_forall()
{
	cout << endl << __FUNCTION__ << endl;
	//let allPositive = Array.forall (fun elem -> elem > 0)
	//printfn "%A" (allPositive [| 0; 1; 2; 3 |])
	//printfn "%A" (allPositive [| 1; 2; 3 |])
	//let allEqual = Array.forall2 (fun elem1 elem2 -> elem1 = elem2)
	//printfn "%A" (allEqual [| 1; 2 |] [| 1; 2 |])
	//printfn "%A" (allEqual [| 1; 2 |] [| 2; 1 |])
	auto allPositive = [](vector<int>& a){
		return fsharp_array::forall([](int elem){return elem > 0;}, a);
	};
	cout << allPositive(vector<int>(list_of(0)(1)(2)(3)))) << endl;
	cout << allPositive(vector<int>(list_of(1)(2)(3)))) << endl;
	auto allEqual = [](vector<int> const& a1, vector<int> const& a2){
		return fsharp_array::forall2([](int elem1, int elem2){return elem1 == elem2;}, a1, a2);
	};
	cout << allEqual(vector<int>(list_of(1)(2)), vector<int>(list_of(1)(2)))) << endl;
	cout << allEqual(vector<int>(list_of(1)(2)), vector<int>(list_of(2)(1)))) << endl;
	//false
	//true
	//true
	//false
}

void test_sort()
{
	cout << endl << __FUNCTION__ << endl;
	//let array1 = [|1; 4; 8; -2; 5|]
	//let sortedArray1 = Array.sort array1
	//printfn "%A" sortedArray1
	vector<int> array1 = list_of(1)(4)(8)(-2)(5);
	auto sortedArray1 = fsharp_array::sort(array1);
	cout << sortedArray1 << endl;
	//[|-2; 1; 4; 5; 8|]

	//let sortedArray2 = Array.sortBy (fun elem -> abs elem) array1
	//printfn "%A" sortedArray2
	auto sortedArray2 = fsharp_array::sortBy([](int elem){return abs(elem);}, array1);
	cout << sortedArray2 << endl;
	//[|1; -2; 4; 5; 8|]

	//let compareElem elem1 elem2 =
	//    if abs elem1 < abs elem2 then -1 else
	//    if abs elem1 = abs elem2 then 0 else
	//    1
	//let sortedArray3 = Array.sortWith compareElem array1
	//printfn "%A" sortedArray3
	auto compareElem = [](int elem1, int elem2){
		return abs(elem1) < abs(elem2) ? -1 : abs(elem1) == abs(elem2) ? 0 : 1;
	};
	auto sortedArray3 = fsharp_array::sortWith(compareElem, array1);
	cout << sortedArray3 << endl;
	//[|1; -2; 4; 5; 8|]

	//let min1 = Array.min array1
	//let max1 = Array.max array1
	//let min2 = Array.minBy (fun elem -> abs elem) array1
	//let max2 = Array.maxBy (fun elem -> abs elem) array1
	//printfn "%d %d %d %d" min1 max1 min2 max2
	auto min1 = fsharp_array::min(array1);
	auto max1 = fsharp_array::max(array1);
	auto min2 = fsharp_array::minBy([](int elem){return abs(elem);}, array1);
	auto max2 = fsharp_array::maxBy([](int elem){return abs(elem);}, array1);
	cout << format("%1% %2% %3% %4%\n") % min1 % max1 % min2 % max2;
	//-2 8 1 8

	//Array.sortInPlace array1
	//printfn "%A" array1
	//Array.sortInPlaceBy (fun elem -> abs elem) array1
	//printfn "%A" array1
	//Array.sortInPlaceWith compareElem array1
	//printfn "%A" array1
	fsharp_array::sortInPlace(array1);
	cout << array1 << endl;
	fsharp_array::sortInPlaceBy([](int elem){return abs(elem);}, array1);
	cout << array1 << endl;
	fsharp_array::sortInPlaceWith(compareElem, array1);
	cout << array1 << endl;
	//[|-2; 1; 4; 5; 8|]
	//[|1; -2; 4; 5; 8|]
	//[|1; -2; 4; 5; 8|]
}

//void test_find()
//{
//	cout << endl << __FUNCTION__ << endl;
//	//let isDivisibleBy number elem = elem % number = 0
//	//let result = Array.find (isDivisibleBy 5) [ 1 .. 100 ]
//	//let result = Array.findIndex (isDivisibleBy 5) [ 1 .. 100 ]
//	//printfn "%d " result
//	vector<int> l(100);
//	iota(l.begin(), l.end(), 1);
//	auto isDivisibleBy = [](int number, int elem){return elem % number == 0;};
//	cout << fsharp_array::find([isDivisibleBy](int elem){return isDivisibleBy(5, elem);}, l) << endl;
//	cout << fsharp_array::findIndex([isDivisibleBy](int elem){return isDivisibleBy(5, elem);}, l) << endl;
//	//5
//	//4
//
//	//let vector1 = [1; 3; 7; 9; 11; 13; 15; 19; 22; 29; 36]
//	//let isEven x = x % 2 = 0
//	//match Array.tryFind isEven vector1 with
//	//    | Some value -> printfn "The first even value is %d." value
//	//    | None -> printfn "There is no even value in the vector."
//	//match Array.tryFindIndex isEven vector1 with
//	//    | Some value -> printfn "The first even value is at position %d." value
//	//    | None -> printfn "There is no even value in the vector."
//
//	vector<int> vector1 = list_of(1)(3)(7)(9)(11)(13)(15)(19)(22)(29)(36);
//	auto isEven = [](int x){return x % 2 == 0;};
//	if(auto result = fsharp_array::tryFind(isEven, vector1))
//		cout << format("The first even value is %1%.\n") % *result;
//	else
//		cout << "There is no even value in the vector.\n";
//	if(auto result = fsharp_array::tryFindIndex(isEven, vector1))
//		cout << format("The first even value is at position %1%.\n") % *result;
//	else
//		cout << "There is no even value in the vector.\n";
//	//The first even value is 22.
//	//The first even value is at position 8.
//
//	//let valuesArray = [ ("a", 1); ("b", 2); ("c", 3) ]
//	//let result = Array.pick (fun elem -> if (snd elem = 2) then Some(fst elem) else None) valuesArray
//	//printfn "%A" result
//	//let result2 = Array.tryPick (fun elem -> if (snd elem = 2) then Some(fst elem) else None) valuesArray
//	//printfn "%A" result2
//	vector<pair<string, int>> valuesArray = list_of<pair<string, int>>("a", 1)("b", 2)("c", 3);
//	auto result = fsharp_array::pick(
//		[](pair<string, int> const& elem){
//			return elem.second == 2 ? optional<string>(elem.first) : optional<string>();
//	}, valuesArray);
//	cout << result << endl;
//	auto result2 = fsharp_array::tryPick(
//		[](pair<string, int> const& elem){
//			return elem.second == 2 ? optional<string>(elem.first) : optional<string>();
//	}, valuesArray);
//	cout << *result2 << endl;
//	//"b"
//	//"b"
//}
//
//void test_other()
//{
//	cout << endl << __FUNCTION__ << endl;
//	//let sum1 = Array.sum [1 .. 10]
//	//let sum2 = Array.sumBy (fun elem -> elem*elem) [1 .. 10]
//	//printfn "%d" sum1
//	//printfn "%d" sum2
//	//let avg1 = Array.average [0.0; 1.0; 1.0; 2.0]
//	//let avg2 = Array.averageBy (fun elem -> float elem) [1 .. 10]
//	//printfn "%f" avg1
//	//printfn "%f" avg2
//
//	vector<int> l(10);
//	iota(l.begin(), l.end(), 1);
//	auto sum1 = fsharp_array::sum(l);
//	auto sum2 = fsharp_array::sumBy([](int elem){return elem * elem;}, l);
//	cout << sum1 << endl;
//	cout << sum2 << endl;
//	auto avg1 = fsharp_array::average(vector<double>(list_of(0.0)(1.0)(1.0)(2.0)));
//	auto avg2 = fsharp_array::averageBy([](int elem) -> double {return elem;}, l);
//	cout << avg1 << endl;
//	cout << avg2 << endl;
//	//55
//	//385
//	//1.0
//	//5.5
//	//let vector1 = [1; 2; 3; 4; 5; 6]
//	//let vector_tuple = Array.partition (fun x -> x % 2 = 0) vector1
//	//printfn "%A" vector_tuple
//	vector<int> vector1 = list_of(1)(2)(3)(4)(5)(6);
//	auto vector_tuple = fsharp_array::partition([](int x){return x % 2 == 0;}, vector1);
//	cout << vector_tuple << endl;
//	//([2; 4; 6], [1; 3; 5])
//
//	//let vectorPermuted = Array.permute (fun x -> 5 - x) vector1
//	//printfn "%A" vectorPermuted
//	auto vectorPermuted = fsharp_array::permute([](size_t x)->size_t{return 5 - x;}, vector1);
//	cout << vectorPermuted << endl;
//	//[6; 5; 4; 3; 2; 1]
//
//	//printfn "%A" (Array.append vector1 vector1)
//	//printfn "%A" Array.empty
//	//printfn "%A" (Array.head vector1)
//	//printfn "%A" (Array.init 3 (fun i -> i * 2))
//	//printfn "%b" (Array.isEmpty vector1)
//	//printfn "%b" (Array.isEmpty Array.empty)
//	//printfn "%d" (Array.length vector1)
//	//printfn "%d" (Array.nth vector1 3)
//	//printfn "%A" (Array.replicate 3 100)
//	//printfn "%A" (Array.rev vector1)
//	//printfn "%A" (Array.tail vector1)
//	cout << fsharp_array::append(vector1, vector1) << endl;
//	cout << fsharp_array::empty<int>() << endl;
//	cout << fsharp_array::head(vector1) << endl;
//	cout << fsharp_array::init(3, [](size_t index){return index * 2;}) << endl;
//	cout << fsharp_array::isEmpty(vector1)) << endl;
//	cout << fsharp_array::isEmpty(fsharp_array::empty<int>())) << endl;
//	cout << fsharp_array::length(vector1) << endl;
//	cout << fsharp_array::nth(vector1, 3) << endl;
//	cout << fsharp_array::replicate(3, 100) << endl;
//	cout << fsharp_array::rev(vector1) << endl;
//	cout << fsharp_array::tail(vector1) << endl;
//	//[1; 2; 3; 4; 5; 6; 1; 2; 3; 4; 5; 6]
//	//[]
//	//1
//	//[0; 2; 4]
//	//false
//	//true
//	//6
//	//4
//	//[100; 100; 100]
//	//[6; 5; 4; 3; 2; 1]
//	//[2; 3; 4; 5; 6]
//}
//
//void test_zip()
//{
//	cout << endl << __FUNCTION__ << endl;
//	//let vector1 = [ 1; 2; 3 ]
//	//let vector2 = [ -1; -2; -3 ]
//	//let vectorZip = Array.zip vector1 vector2
//	//printfn "%A" vectorZip
//	//let vector3 = [ 0; 0; 0]
//	//let vectorZip3 = Array.zip3 vector1 vector2 vector3
//	//printfn "%A" vectorZip3
//
//	vector<int> vector1 = list_of(1)(2)(3);
//	vector<int> vector2 = list_of(-1)(-2)(-3);
//	auto vectorZip = fsharp_array::zip(vector1, vector2);
//	cout << vectorZip << endl;
//	vector<int> vector3 = list_of(0)(0)(0);
//	auto vectorZip3 = fsharp_array::zip3(vector1, vector2, vector3);
//	cout << vectorZip3 << endl;
//	//[(1, -1); (2, -2); (3, -3)]
//	//[(1, -1, 0); (2, -2, 0); (3, -3, 0)]
//
//	//let vectors = Array.unzip [(1,2); (3,4)]
//	//printfn "%A" vectors
//	//printfn "%A %A" (fst vectors) (snd vectors)
//	//let vectorsUnzip3 = Array.unzip3 [(1,2,3); (4,5,6)]
//	//printfn "%A" vectorsUnzip3
//	auto vectors = fsharp_array::unzip(vector<tuple<int, int>>(tuple_list_of(1, 2)(3, 4)));
//	cout << vectors << endl;
//	cout << format("%1% %2%\n") % get<0>(vectors) % get<1>(vectors);
//	auto vectorsUnzip3 = fsharp_array::unzip3(vector<tuple<int, int, int>>(tuple_list_of(1, 2, 3)(4, 5, 6)));
//	cout << vectorsUnzip3 << endl;
//	//([1; 3], [2; 4])
//	//[1; 3] [2; 4]
//	//([1; 4], [2; 5], [3; 6])
//}
//
//void test_iter()
//{
//	cout << endl << __FUNCTION__ << endl;
//	//let vectors = Array.unzip [(1,2); (3,4)]
//	//printfn "%A" vectors
//	//printfn "%A %A" (fst vectors) (snd vectors)
//	//let vectorsUnzip3 = Array.unzip3 [(1,2,3); (4,5,6)]
//	//printfn "%A" vectorsUnzip3
//	auto vectors = fsharp_array::unzip(vector<tuple<int, int>>(tuple_list_of(1, 2)(3, 4)));
//	cout << vectors << endl;
//	cout << format("%1% %2%\n") % get<0>(vectors) % get<1>(vectors);
//	auto vectorsUnzip3 = fsharp_array::unzip3(vector<tuple<int, int, int>>(tuple_list_of(1, 2, 3)(4, 5, 6)));
//	cout << vectorsUnzip3 << endl;
//	//([1; 3], [2; 4])
//	//[1; 3] [2; 4]
//	//([1; 4], [2; 5], [3; 6])
//}
//
//void test_map()
//{
//	cout << endl << __FUNCTION__ << endl;
//	//let vector1 = [1; 2; 3]
//	//let newArray = Array.map (fun x -> x + 1) vector1
//	//printfn "%A" newArray
//	//let vector2 = [4; 5; 6]
//	//let sumArray = Array.map2 (fun x y -> x + y) vector1 vector2
//	//printfn "%A" sumArray
//	//let newArray2 = Array.map3 (fun x y z -> x + y + z) vector1 vector2 [2; 3; 4]
//	//printfn "%A" newArray2
//	//let newArrayAddIndex = Array.mapi (fun i x -> x + i) vector1
//	//printfn "%A" newArrayAddIndex
//	//let vectorAddTimesIndex = Array.mapi2 (fun i x y -> (x + y) * i) vector1 vector2
//	//printfn "%A" vectorAddTimesIndex
//
//	vector<int> vector1 = list_of(1)(2)(3);
//	auto newArray = fsharp_array::map([](int x){return x + 1;}, vector1);
//	cout << newArray << endl;
//	vector<int> vector2 = list_of(4)(5)(6);
//	auto sumArray = fsharp_array::map2([](int x, int y){return x + y;}, vector1, vector2);
//	cout << sumArray << endl;
//	auto newArray2 = fsharp_array::map3([](int x, int y, int z){return x + y + z;},
//		vector1, vector2, vector<int>(list_of(2)(3)(4)));
//	cout << newArray2 << endl;
//	auto newArrayAddIndex = fsharp_array::mapi([](size_t i, int x){return x + i;}, vector1);
//	cout << newArrayAddIndex << endl;
//	auto vectorAddTimesIndex = fsharp_array::mapi2([](size_t i, int x, int y) -> int{return (x + y) * i;}, vector1, vector2);
//	cout << vectorAddTimesIndex << endl;
//	//[2; 3; 4]
//	//[5; 7; 9]
//	//[7; 10; 13]
//	//[1; 3; 5]
//	//[0; 7; 18]
//
//	//let collectArray = Array.collect (fun x -> [for i in 1..3 -> x * i]) vector1
//	//printfn "%A" collectArray
//	auto collectArray = fsharp_array::collect(
//		[](int x) -> vector<int> {
//			vector<int> l;
//			for(int i = 1; i <= 3; i++)
//				l.push_back(x * i);
//			return l;
//		}, vector1
//	);
//	cout << collectArray << endl;
//	//[1; 2; 3; 2; 4; 6; 3; 6; 9]
//
//	//let evenOnlyArray = Array.filter (fun x -> x % 2 = 0) [1; 2; 3; 4; 5; 6]
//	//printfn "%A" evenOnlyArray
//	auto evenOnlyArray = fsharp_array::filter(
//		[](int x){return x % 2 == 0;}, vector<int>(list_of(1)(2)(3)(4)(5)(6))
//	);
//	cout << evenOnlyArray << endl;
//	//[2; 4; 6]
//
//	//let vectorWords = [ "and"; "Rome"; "Bob"; "apple"; "zebra" ]
//	//let isCapitalized (string1:string) =
//	//    string1.Chars(0) >= 'A' && string1.Chars(0) <= 'Z'
//	//let results = Array.choose (fun elem -> if isCapitalized elem then Some(elem) else None) vectorWords
//	//printfn "%A" results
//	vector<string> vectorWords = list_of<string>("and")("Rome")("Bob")("apple")("zebra");
//	auto isCapitalized = [](string const& str){return str[0] >= 'A' && str[0] <= 'Z';};
//	auto results = fsharp_array::choose(
//		[&](string const& elem){
//			return isCapitalized(elem) ? optional<string>(elem) : optional<string>();
//		}, vectorWords
//	);
//	cout << results << endl;
//	//[Rome; Bob]
//}
//
//void test_fold()
//{
//	cout << endl << __FUNCTION__ << endl;
//	//let sumArray vector = Array.fold (fun acc elem -> acc + elem) 0 vector
//	//printfn "Sum of the elements of vector %A is %d." [ 1 .. 3 ] (sumArray [ 1 .. 3 ])
//	vector<int> vector1 = list_of(1)(2)(3);
//	auto sumArray = [](vector<int> const& l){return fsharp_array::fold(
//		[](int acc, int elem){return acc + elem;}, 0, l
//	);};
//	cout << format("Sum of the elements of vector %1% is %2%.\n") % vector1 % sumArray(vector1);
//	//Sum of the elements of vector [1; 2; 3] is 6.
//
//	//let averageArray vector = (Array.fold (fun acc elem -> acc + float elem) 0.0 vector / float vector.Length)
//	//let stdDevArray vector =
//	//    let avg = averageArray vector
//	//    sqrt (Array.fold (fun acc elem -> acc + (float elem - avg) ** 2.0 ) 0.0 vector / float vector.Length)
//	//let testArray vectorTest =
//	//    printfn "Array %A average: %f stddev: %f" vectorTest (averageArray vectorTest) (stdDevArray vectorTest)
//	//testArray [1; 1; 1]
//	//testArray [1; 2; 1]
//	//testArray [1; 2; 3]
//	auto averageArray = [](vector<int> const& l){
//		return fsharp_array::fold(
//			[](double acc, int elem){return acc + elem;}, 0.0, l
//		) / l.size();
//	};
//	auto stdDevArray = [&](vector<int> const& l) -> double {
//		auto avg = averageArray(l);
//		return sqrt(fsharp_array::fold(
//			[avg](double acc, int elem){return acc + (elem - avg) * (elem - avg);}, 0.0, l
//		) / l.size());
//	};
//	auto testArray = [&](vector<int> const& l){
//		cout << format("Array %1% average: %2% stddev: %3%\n") % l % averageArray(l) % stdDevArray(l);
//	};
//	testArray(vector<int>(list_of(1)(1)(1)));
//	testArray(vector<int>(list_of(1)(2)(1)));
//	testArray(vector<int>(list_of(1)(2)(3)));
//	//Array [1; 1; 1] average: 1 stddev: 0
//	//Array [1; 2; 1] average: 1.33333 stddev: 0.471405
//	//Array [1; 2; 3] average: 2 stddev: 0.816497
//
//	//let printArray vector = Array.fold (fun acc elem -> printfn "%A" elem) () vector
//	//printArray [0.0; 1.0; 2.5; 5.1 ]
//	auto printArray = [](vector<double> const& l){fsharp_array::fold(
//		[](int acc, double elem){return cout << elem << endl, acc;}, 0, l
//	);};
//	printArray(vector<double>(list_of(0.0)(1.0)(2.5)(5.1)));
//	//0
//	//1
//	//2.5
//	//5.1
//
//	//let reverseArray vector = Array.fold (fun acc elem -> elem::acc) [] vector
//	//printfn "%A" (reverseArray [1 .. 10])
//	vector<int> vector2(10);
//	iota(vector2.begin(), vector2.end(), 1);
//	auto reverseArray = [](vector<int> const& l){
//		return fsharp_array::fold(
//			[](vector<int> acc, int elem){return acc.push_front(elem), acc;}, vector<int>(), l
//		);};
//	cout << reverseArray(vector2) << endl;
//	//[10; 9; 8; 7; 6; 5; 4; 3; 2; 1]
//
//	//let sumArrayBack vector = Array.foldBack (fun elem acc -> elem + acc) vector 0
//	//printfn "%d" (sumArrayBack [1; 2; 3])
//	//let copyArray vector = Array.foldBack (fun elem acc -> elem::acc) vector []
//	//printfn "%A" (copyArray [1 .. 10])
//	auto sumArrayBack = [](vector<int> const& l){
//		return fsharp_array::foldBack(
//			[](int elem, int acc){return elem + acc;}, l, 0
//		);};
//	cout << sumArrayBack(vector<int>(list_of(1)(2)(3))) << endl;
//	auto copyArray = [](vector<int> const& l){
//		return fsharp_array::foldBack(
//			[](int elem, vector<int> acc){return acc.push_front(elem), acc;}, l, vector<int>()
//		);};
//	cout << copyArray(vector2) << endl;
//	//[1; 2; 3; 4; 5; 6; 7; 8; 9; 10]
//
//	//// Discriminated union type that encodes the transaction type.
//	//type Transaction =
//	//    | Deposit
//	//    | Withdrawal
//	//let transactionTypes = [Deposit; Deposit; Withdrawal]
//	//let transactionAmounts = [100.00; 1000.00; 95.00 ]
//	//let initialBalance = 200.00
//	//// Use fold2 to perform a calculation on the vector to update the account balance.
//	//let endingBalance = Array.fold2 (fun acc elem1 elem2 ->
//	//                                match elem1 with
//	//                                | Deposit -> acc + elem2
//	//                                | Withdrawal -> acc - elem2)
//	//                                initialBalance
//	//                                transactionTypes
//	//                                transactionAmounts
//	//printfn "%f" endingBalance
//	enum Transaction {Deposit, Withdrawal};
//	vector<Transaction> transactionTypes = list_of<Transaction>(Deposit)(Deposit)(Withdrawal);
//	vector<double> transactionAmounts = list_of(100.00)(1000.00)(95.00);
//	auto initialBalance = 200.00;
//	auto endingBalance = fsharp_array::fold2(
//		[=](double acc, Transaction elem1, double elem2) -> double {
//			return elem1 == Deposit ? acc + elem2 : acc - elem2;
//		}, 
//		initialBalance, transactionTypes, transactionAmounts
//	);
//	cout << format("%f\n") % endingBalance;
//	//1205
//
//	//type Transaction2 =
//	//| Deposit
//	//| Withdrawal
//	//| Interest
//	//
//	//let transactionTypes2 = [Deposit; Deposit; Withdrawal; Interest]
//	//let transactionAmounts2 = [100.00; 1000.00; 95.00; 0.05 / 12.0 ]
//	//let initialBalance2 = 200.00
//	//
//	//// Because fold2 processes the vectors by starting at the head element,
//	//// the interest is calculated last, on the balance of 1205.00.
//	//let endingBalance2 = Array.fold2 (fun acc elem1 elem2 ->
//	//                                match elem1 with
//	//                                | Deposit -> acc + elem2
//	//                                | Withdrawal -> acc - elem2
//	//                                | Interest -> acc * (1.0 + elem2))
//	//                                initialBalance2
//	//                                transactionTypes2
//	//                                transactionAmounts2
//	//printfn "%f" endingBalance2
//	//
//	//// Because foldBack2 processes the vectors by starting at end of the vector,
//	//// the interest is calculated first, on the balance of only 200.00.
//	//let endingBalance3 = Array.foldBack2 (fun elem1 elem2 acc ->
//	//                                match elem1 with
//	//                                | Deposit -> acc + elem2
//	//                                | Withdrawal -> acc - elem2
//	//                                | Interest -> acc * (1.0 + elem2))
//	//                                transactionTypes2
//	//                                transactionAmounts2
//	//                                initialBalance2
//	//printfn "%f" endingBalance3
//	enum Transaction2 {Deposit2, Withdrawal2, Interest2};
//	vector<Transaction2> transactionTypes2 = list_of<Transaction2>(Deposit2)(Deposit2)(Withdrawal2)(Interest2);
//	vector<double> transactionAmounts2 = list_of(100.00)(1000.00)(95.00)(0.05 / 12.0);
//	auto initialBalance2 = 200.00;
//	auto endingBalance2 = fsharp_array::fold2(
//		[=](double acc, Transaction2 elem1, double elem2) -> double {
//			return elem1 == Deposit2 ? acc + elem2 :
//				elem1 == Withdrawal2 ? acc - elem2 :
//				acc * (1.0 + elem2);
//		}, initialBalance2, transactionTypes2, transactionAmounts2
//	);
//	cout << format("%f\n") % endingBalance2;
//	auto endingBalance3 = fsharp_array::foldBack2(
//		[=](Transaction2 elem1, double elem2, double acc) -> double {
//			return elem1 == Deposit2 ? acc + elem2 :
//				elem1 == Withdrawal2 ? acc - elem2 :
//				acc * (1.0 + elem2);
//		}, transactionTypes2, transactionAmounts2, initialBalance2
//	);
//	cout << format("%f\n") % endingBalance3;
//	//1210.020833
//	//1205.833333
//
//	//let vector1 = [1; 2; 3]
//	//let vectorState = Array.scan (fun acc elem -> acc + elem) 0 vector1
//	//printfn "%A" vectorState
//	//let vectorState2 = Array.scanBack (fun elem acc -> elem + acc) vector1 0
//	//printfn "%A" vectorState2
//	auto vectorState = fsharp_array::scan(
//		[](int acc, int elem){return acc + elem;}, 0, vector1
//	);
//	cout << vectorState << endl;
//	auto vectorState2 = fsharp_array::scanBack(
//		[](int elem, int acc){return elem + acc;}, vector1, 0
//	);
//	cout << vectorState2 << endl;
//	//[0; 1; 3; 6]
//	//[6; 5; 3; 0]
//
//	//let n1 = Array.reduce (fun acc elem -> acc - elem) vector1
//	//let n2 = Array.reduceBack (fun elem acc -> elem - acc) vector1
//	//printfn "%d %d" n1 n2
//	auto n1 = fsharp_array::reduce([](int acc, int elem){return acc - elem;}, vector1);
//	auto n2 = fsharp_array::reduceBack([](int elem, int acc){return elem - acc;}, vector1);
//	cout << format("%1% %2%\n") % n1 % n2;
//	//-4 2
//}

int main()
{
	test_exists();
	test_forall();
	test_sort();
	//test_find();
	//test_zip();
	//test_iter();
	//test_map();
	//test_fold();
	//test_other();
	return 0;
}


