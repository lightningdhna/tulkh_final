#include "Judge.hpp"
#include "Problem.hpp"
#include "BranchAndBoundAlgorithm.hpp"
#include "LargeNeighborhoodSearch.hpp"

#include <iostream>
#include <algorithm>
#include<chrono>



#define get_time() high_resolution_clock::now()

using namespace std;
using namespace chrono;
auto start_time = get_time();


Problem problem;
Judge judge(problem);

inline long long cal_time_ms()
{
	const auto cur = get_time();
	return duration_cast<nanoseconds>(cur - start_time).count() / 1e6;
}



int main()
{
	freopen_s(new FILE*, "input.txt", "r", stdin);
	//freopen_s(new FILE*, "output.txt", "w", stdout);

	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	problem.read_input();
	cout << endl << cal_time_ms() << " ms" << endl;
	judge = Judge(problem);
	//judge.print_input_solution("my_output.txt");
	//cout << judge.cal_score(BAB(problem).solve());

	auto algo = LNS(problem);
	algo.solve();
	algo.print_solution();
	cout << judge.cal_score(algo.best_solution);
	cout << endl << cal_time_ms() << " ms" << endl;

	int a;
}