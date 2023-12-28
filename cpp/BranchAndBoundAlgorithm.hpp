#pragma once
#include "Problem.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class BAB
{

public:

	vector<vector<int>> best_solution;
	vector<bool> vis;
	vector<vector<int>> cur_routes;
	int best_score = 1e9;
	int vis_counter = 0;
	vector<int> len;
	int n, k;
	vector<vector<int>> dis;
	BAB(const Problem& problem)
	{
		n = problem.vertices_number;
		k = problem.postman_number;
		dis = problem.distance_matrix;

		cur_routes.resize(k, { 0 });
		vis.resize(n + 1, false);
		len.resize(k, 0);
		vis_counter = 0;


		best_score = 1e9;
	}
	bool check(const int pid, const int v) const
	{
		if (vis[v])
			return false;
		const int prev_v = cur_routes[pid].back();
		if (pid == 0)
		{
			if (len[0] + dis[prev_v][v] >= best_score)
			{
				return false;
			}
		}
		else
		{
			if (len[pid] + dis[prev_v][v] > len[pid - 1])
			{
				return false;
			}
		}
		return true;
	}
	/**
	 * \brief
	 * \param pid postman id
	 */
	void search(const int pid)
	{
		if (vis_counter == n)
		{
			if (best_score > len[0])
			{
				best_score = len.front();
				best_solution = cur_routes;
				return;
			}
		}
		if (pid < k - 1)
			search(pid + 1);
		for (int i = 1; i <= n; i++)
		{
			if (!check(pid, i))
				continue;

			auto const c = dis[cur_routes[pid].back()][i];

			vis[i] = true;
			len[pid] += c;
			cur_routes[pid].push_back(i);
			++vis_counter;

			search(pid);

			--vis_counter;
			cur_routes[pid].pop_back();
			len[pid] -= c;
			vis[i] = false;

		}

	}

	void print_best_routes() const
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
	void print_solution() const
	{
		print_best_routes();
	}

	vector<vector<int>> solve()
	{
		search(0);
		return best_solution;
	}
};