#include "stdafx.h"
#include <iostream>
#include <vector>
#include "mycpplinq.hpp"
#define BOOST_TEST_MODULE mycpplinq test
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(from_where_select_to)
{
	int a[] = {1, 2, 3, 4};
	// 找出数组a中所有的偶数，乘以2，将结果存放到一个vector对象中
	auto v = from(a) >> where([](int n){return n % 2 == 0;}) >> select([](int n){return n * 2;}) >> to_vector();
	//for(int i : v) std::cout << i << std::endl;
	std::vector<int> v2 = {4, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(v.begin(), v.end(), v2.begin(), v2.end());
}
