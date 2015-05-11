// cpplinqtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <locale.h>
#include "cpplinqex.hpp"

using namespace std;
using namespace cpplinq;

void print(wstring const& text, int offset)
{
	//auto lu = from(text)
	//	>> zip_with(range(0, text.length()))
	//	>> to_lookup([=](pair<wchar_t, int> const& p){return p.second % offset;});
	//
	//for(auto const& kv : lu.keys){
	//	auto v = lu[kv.first]
	//		>> select([](pair<wchar_t, int> const& p){return wstring(1, p.first);})
	//		>> reverse()
	//		>> concatenate(L"|");
	//	wcout << v << endl;
	//}
	//from(text)
	//	>> zip_with(range(0, text.length()))
	//	>> groupby([=](pair<wchar_t, int> const& p){return p.second % offset;})
	//	>> for_each([](pair<int, vector<pair<wchar_t, int>>> const& kvs){
	//		auto v = from(kvs.second)
	//			>> select([](pair<wchar_t, int> const& p){return wstring(1, p.first);})
	//			>> reverse()
	//			>> concatenate(L"|");
	//		wcout << v << endl;
	//	});
	//from(text)
	//	>> zip_with(range(0, text.length()))
	//	>> groupby([=](pair<wchar_t, int> const& p){return p.second % offset;},
	//		[](pair<wchar_t, int> const& p){return wstring(1, p.first);})
	//	>> for_each([](pair<int, vector<wstring>> const& kvs){
	//		auto v = from(kvs.second)
	//			>> reverse()
	//			>> concatenate(L"|");
	//		wcout << v << endl;
	//	});
	auto groupings = from(text)
		>> zip_with(range(0, text.length()))
		>> to_groups([=](pair<wchar_t, int> const& p){return p.second % offset;},
			[](pair<wchar_t, int> const& p){return wstring(1, p.first);});
	for(auto const& kvs : groupings){
		auto v = from(kvs.second)
			>> reverse()
			>> concatenate(L"|");
		wcout << v.c_str() << endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int a[] = {1, 2, 3, 4};
	int s = from_array(a)
		>> aggregate([](int acc, int elem){return acc + elem;});
	wcout << s << endl;

	setlocale(LC_ALL, "chs" );
	print(L"床前明月光疑似地上霜举头望明月低头思故乡", 5);
	return 0;
}

