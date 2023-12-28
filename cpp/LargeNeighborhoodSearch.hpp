#pragma once
#include "Problem.hpp"

#include<iostream>
#include<cmath>
#include<algorithm>
#include<random>

#include "InitializeSolutionAlgorithm.hpp"

#define Route vector<int>
#define Solution vector<Route>
#define LenSolution vector<pair<int,Route>>

using namespace std;

class LNS
{

public:
	int n, k;
	vector<vector<int>> dis;

	Solution best_solution;
	int best_score;
	int max_edge;
	Problem problem;
	LNS(const Problem& problem)
	{
		this->problem = problem;
		n = problem.vertices_number;
		k = problem.postman_number;
		dis = problem.distance_matrix;
		max_edge = problem.max_edge;
	}

	Solution solve()
	{
		// LNS
		//init
		// loop
			// destroy
			// repair
			// update ( SA)
		best_solution = cw_algorithm_init(problem);
		while (cycle_and_cut(best_solution)) {}

		//swap 2 route
		LenSolution len_sol = convert(best_solution);

		int cnt = 0;
		vector<bool> change(k, true);
		vector<vector<int>> lens(k);
		for (int i = 0; i < k; i++) lens[i] = cal_len_array(len_sol[i].second);
		while (cross_route(len_sol, change, lens))
		{

		}
		best_solution = convert(len_sol);
		best_score = cal_score(best_solution);
		vector<int> unpick = {};
		// 2opt
		// lns


		//for(int i=0;i<10;i++)
		//{
		//	local_search(current_solution, unpick);
		//	// SA

		//}

		for (int i = 0; i < 5000; i++)
		{
			lns_small(len_sol, unpick);
			int max_len = 0;
			for (int pm = 0; pm < k; pm++) max_len = max(max_len, len_sol[pm].first);
			if (best_score > max_len && unpick.empty())
			{
				best_solution = convert(len_sol);
				best_score = max_len;
			}
			//update condition here
			//terminate condition here
		}

		return best_solution;

	}

	int cal_len(const Route& route) const
	{
		int len = 0;
		int cur = 0;
		for (const int v : route)
		{
			len += dis[cur][v];
			cur = v;
		}
		return len;
	}

	int cal_score(const Solution& sol) const
	{
		int max_len = 0;
		for (auto& route : sol)
		{
			max_len = max(max_len, cal_len(route));
		}
		return max_len;
	}

	vector<int> cal_len_array(const Route& route) const
	{
		vector<int> res;
		res.reserve(route.size());
		int len = 0;
		int cur = 0;
		for (const int v : route)
		{
			len += dis[cur][v];
			res.push_back(len);
			cur = v;
		}
		return res;
	}

	// n+2 là end
	LenSolution convert(const Solution& sol)const
	{
		LenSolution res;
		res.reserve(sol.size());
		for (const auto& route : sol)
		{

			res.push_back({ cal_len(route),route });
			res.back().second.push_back(n + 2);
		}
		return res;
	}

	Solution convert(const LenSolution& sl)const
	{
		Solution res;
		res.reserve(sl.size());
		for (const auto& tem : sl)
		{
			res.push_back(tem.second);
			res.back().pop_back();
		}
		return res;
	}
	static void sort_routes(LenSolution& sl)
	{
		sort(sl.begin(), sl.end(), [](auto a, auto b)
			{
				return a.first < b.first;
			});

	}
	/**
	 * \brief
	 * \param short_route
	 * \param long_route
	 */
	bool cycle_and_cut(Route& short_route, Route& long_route) const
	{

		int sl = cal_len(short_route), ll = cal_len(long_route);


		//todo check nếu cách làm này thật sự ổn
		if (ll < sl)
		{
			swap(ll, sl);
			swap(long_route, short_route);
		}



		int best_result = max(sl, ll);
		const int prev_result = best_result;
		while (sl < ll && long_route.size()>1)
		{
			sl += dis[short_route.back()][long_route.back()];
			ll -= dis[long_route[long_route.size() - 2]][long_route.back()];
			short_route.push_back(long_route.back());
			long_route.pop_back();
			best_result = min(best_result, max(sl, ll));
		}
		if (sl > best_result)
		{
			long_route.push_back(short_route.back());
			short_route.pop_back();
		}
		return best_result < prev_result;
	}

	bool cycle_and_cut(Solution& sol) const
	{
		vector<pair<int, pair<int, int>>> ps;
		vector<int> lens(k, 0);
		for (int i = 0; i < k; i++)
			lens[i] = cal_len(sol[i]);
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < k; j++)
			{
				if (i ^ j)
				{
					int score = lens[j] - lens[i] - dis[sol[i].back()][sol[j].back()];
					ps.push_back({ score,{i,j} });
				}
			}
		}
		sort(ps.begin(), ps.end(), [](auto a, auto b)->bool
			{
				return a.first > b.first;
			});

		vector<bool> done(k, false);
		bool can_improve = false;
		for (const auto& p : ps)
		{
			const int fi = p.second.first;
			const int se = p.second.second;
			if (done[fi] || done[se])
				continue;
			can_improve |= cycle_and_cut(sol[fi], sol[se]);
			done[fi] = true;
			done[se] = true;
		}
		return can_improve;
	}

	bool cross(Route& a, Route& b, int& len1, int& len2, vector<int>& lena, vector<int>& lenb) const
	{


		//mục tiêu là giảm max(lena, lenb)
		const int size_a = a.size();
		const int size_b = b.size();
		const int max_len = max(lena.back(), lenb.back());
		pair<int, int> best_swap;
		pair<int, int> new_len;
		int best_dec = 0;
		for (int i = 2; i < size_a; i++)
		{
			for (int j = 2; j < size_b; j++)
			{
				const int new_lena = lena[i - 1] + lenb.back() - lenb[j] + dis[a[i - 1]][b[j]];
				const int new_lenb = lenb[j - 1] + lena.back() - lena[i] + dis[b[j - 1]][a[i]];
				const int dec = max_len - max(new_lena, new_lenb);
				if (dec > best_dec)
				{
					best_dec = dec;
					best_swap = { i, j };
					new_len = { new_lena,new_lenb };
				}
			}
		}
		//swap i,j
		if (best_dec == 0) return false;
		const int u = best_swap.first;
		const int v = best_swap.second;
		vector<int> tem(b.begin() + v, b.end());
		b.erase(b.begin() + v, b.end());
		b.insert(b.end(), a.begin() + u, a.end());
		a.erase(a.begin() + u, a.end());
		a.insert(a.end(), tem.begin(), tem.end());
		len1 = new_len.first;
		len2 = new_len.second;
		return true;
	}

	bool cross_route(LenSolution& sl, vector<bool>& change, vector<vector<int>>& lens) const
	{
		//sort_routes(sl);

		vector<pair<double, pair<int, int>>> pri_order;
		pri_order.reserve(k * (k - 1) / 2 + 1);
		vector<bool> done(k, false);
		for (int i = 0; i < k; i++)
		{
			for (int j = i + 1; j < k; j++)
			{
				//cal heuristic
				double heu = abs(sl[i].first - sl[j].first);
				double cor = 0;
				for (const auto u : sl[i].second)
				{
					for (const auto v : sl[j].second)
					{
						cor += 1.0 / (1 + dis[u][v]);
					}
				}
				heu *= cor;

				pri_order.push_back({ heu,{i,j} });
			}
		}
		sort(pri_order.begin(), pri_order.end(), [](auto a, auto b)->bool
			{
				return a.first > b.first;
			});
		bool can_improve = false;
		for (const auto& p : pri_order)
		{
			const int i = p.second.first;
			const int j = p.second.second;
			if (done[i] || done[j])
				continue;
			if (!(change[i] || change[j]))
				continue;
			if (cross(sl[i].second, sl[j].second, sl[i].first, sl[j].first, lens[i], lens[j]))
			{
				can_improve = true;
				done[i] = true;
				done[j] = true;
				lens[i] = cal_len_array(sl[i].second);
				lens[j] = cal_len_array(sl[j].second);
			}
		}
		for (int i = 0; i < k; i++)
		{
			if (done[i])
				change[i] = true;
			else change[i] = false;
		}
		return can_improve;
	}


	Solution split_route(const Route& r) const
	{
		auto check = [=](const int mid)->bool
			{
				int k_need = 1;
				int cur = 0;
				int len = 0;
				for (int i = 1; i <= n; i++)
				{
					const int v = r[i];
					const int  d = dis[cur][v];
					if (d > mid)
						return false;
					if (len + d > mid)
					{
						len = dis[0][v];
						cur = v;
						++k_need;
						if (k_need > k)
							return false;
					}
					else
					{
						len += d;
						cur = v;
					}
				}
				return true;
			};
		auto apply_split = [&](const int max_len)->Solution
			{
				Solution sol;
				int cur = 0;
				int len = 0;
				sol.push_back({ 0 });
				for (int i = 1; i <= n; i++)
				{
					const int v = r[i];
					const int  d = dis[cur][v];
					if (len + d > max_len)
					{
						len = dis[0][v];
						cur = v;
						sol.push_back({ 0,v });
					}
					else
					{
						len += d;
						cur = v;
						sol.back().push_back(v);
					}
				}
				return sol;
			};

		int left = 0, right = 1e6;
		// todo make right-left smaller

		while (left < right)
		{
			const int mid = (left + right) >> 1;
			if (check(mid))
			{
				right = mid;
			}
			else
			{
				left = mid + 1;
			}
		}
		return apply_split(left);
	}

	Solution random_init() const
	{
		Route r(n + 1);
		for (int i = 1; i <= n; i++)
			r[i] = i;
		random_device rd_;
		mt19937 gen_(rd_());
		shuffle(r.begin() + 1, r.end(), gen_);
		return split_route(r);
	}


	void local_search(Solution& cur, vector<int>& unpick)
	{

	}


	void print_solution() const
	{
		cout << best_solution.size() << endl;
		for (const auto& route : best_solution)
		{
			cout << route.size() << endl;
			for (auto& v : route)
			{
				cout << v << " ";
			}
			cout << endl;
		}
	}

	bool destroy_one(Route& route, vector<int>& unpick, int& len)const
	{

		//route này ó điểm cuối là n+2, đánh dấu vị trí kết thúc

		int best_dec = -1;
		int best_idx = 1;
		if (route.size() <= 2)return false;
		for (int i = 1; i<int(route.size()) - 1; i++)
		{
			const int score = dis[route[i - 1]][route[i]] + dis[route[i]][route[i + 1]] - dis[route[i - 1]][route[i + 1]];
			if (score > best_dec)
			{
				best_dec = score;
				best_idx = i;
			}
		}
		if (best_dec <= 0)
			return false;
		unpick.push_back(route[best_idx]);
		route.erase(route.begin() + best_idx);
		len = cal_len(route);
		return true;
	}
	bool insert(const int id, LenSolution& ls) const
	{
		int best_inc = 1e9;
		pair<int, int> best_pos;
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j<int(ls[i].second.size()) - 1; j++)
			{
				const int u = ls[i].second[j];
				const int v = ls[i].second[j + 1];
				const int inc = dis[u][id] + dis[v][id] - dis[u][v]
					+ ls[i].first * (1)
					;
				if (inc < best_inc)
				{
					best_inc = inc;
					best_pos = { i,j };
				}
			}
		}
		if (best_inc >= 1e9 - 1)
			return false;
		const int i = best_pos.first;
		const int j = best_pos.second;
		ls[i].second.insert(ls[i].second.begin() + (j + 1), id);
		ls[i].first = cal_len(ls[i].second);
		return true;
	}


	void lns_small(LenSolution& ls, vector<int>& unpick) const
	{
		//init 	
		sort_routes(ls);
		double sum_len = 0;
		for (int i = 0; i < k; i++)
		{
			sum_len += ls[i].first;
		}
		if (sum_len <= 0)return;
		vector<double> pnt(k);
		for (int i = 0; i < k; i++)
			pnt[i] = ls[i].first / sum_len;

		//select and destroy
		//todo tune 2 tham số này, 5 và 3
		for (int i = 0; i < 5; i++)
		{
			double rd = double(rand()) / RAND_MAX;
			int selected_idx = 0;
			while (selected_idx < k)
			{
				rd -= pnt[selected_idx];
				if (rd <= 0)
					break;
				++selected_idx;
			}
			destroy_one(ls[selected_idx].second, unpick, ls[selected_idx].first);
			destroy_one(ls[selected_idx].second, unpick, ls[selected_idx].first);
			destroy_one(ls[selected_idx].second, unpick, ls[selected_idx].first);
		}


		//destroy

		//insert using saving condition
		// later we care about cluster

		for (int i = int(unpick.size()) - 1; i >= 0; i--)
		{
			if (insert(unpick[i], ls))
				unpick.pop_back();
		}

	}


};
