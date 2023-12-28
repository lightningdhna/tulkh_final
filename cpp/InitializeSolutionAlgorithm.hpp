#pragma once

#include<iostream>
#include<cmath>
#include<algorithm>
#include<random>
#include<vector>

#include "DSU.hpp"
#include "Problem.hpp"


#define Route vector<int>
#define Solution vector<Route>

using namespace std;

inline Solution cw_algorithm_init(const Problem& problem) 
{
	const int vertices_number = problem.vertices_number;
	const int postman_number = problem.postman_number;
	const int max_edge = problem.max_edge;
	const auto distance_matrix = problem.distance_matrix;

	int bound = 20 * vertices_number / postman_number;
	int max_len = max_edge;

	vector<int> prev(vertices_number + 1, 0);
	vector<bool> is_last(vertices_number + 1, true);
	vector<vector<int>> saving_mp(vertices_number + 1);
	DSU dsu(vertices_number + 1);
	for (int i = 1; i <= vertices_number; i++)
		dsu.change_len(i, distance_matrix[0][i]);

	// create saving_list
	vector<pair<int, pair<int, int>>> saving_list;
	for (int i = 1; i <= vertices_number; i++)
	{
		saving_mp[i].resize(vertices_number + 1);
		for (int j = 1; j <= vertices_number; j++)
		{
			const int score = distance_matrix[0][j] - distance_matrix[i][j];
			saving_mp[i][j] = score;
			if (i ^ j)
				saving_list.push_back({ score,{i,j} });
		}
	}
	sort(saving_list.begin(), saving_list.end(), [](auto a, auto b)->bool
		{
			return a.first < b.first;
		});
	vector<bool> skip(saving_list.size() + 1, false);


	// merge
	int merge_num = vertices_number - postman_number;
	while (merge_num)
	{

		//search saving_list
		int change = 0;
		for (int idx = static_cast<int>(saving_list.size()) - 1; idx >= 0 && merge_num; idx--)
		{

			if (skip[idx])
				continue;
			const auto saving_pair = saving_list[idx];
			const int saving_score = saving_pair.first;
			const int i = saving_pair.second.first;
			const int j = saving_pair.second.second;

			//check possible to merge
			if (!is_last[i])
			{
				skip[idx] = true;
				continue;
			}
			if (prev[j])
			{
				skip[idx] = true;
				continue;
			}
			if (!dsu.can_join(i, j))
			{
				skip[idx] = true;
				continue;
			}
			const int new_len = dsu.get_len(i) + dsu.get_len(j) - saving_score;
			if (new_len > bound)
				continue;

			//merge & change
			--merge_num;
			dsu.join(i, j);

			dsu.change_len(i, new_len);
			prev[j] = i;
			is_last[i] = false;

			max_len = max(max_len, new_len);
			change = 1;




		}
		//todo thay đổi cách tăng dần của biến bound
		bound += (1 - change) * (max_len + 200);
	}
	vector<Route> res;
	vector<int> next(vertices_number + 1, -1);
	for (int i = 1; i <= vertices_number; i++)
	{
		if (prev[i] == 0)
		{
			res.push_back({ 0,i });
		}
		else
		{
			next[prev[i]] = i;
		}
	}
	for (Route& route : res)
	{
		int last = route.back();
		last = next[last];
		while (last != -1)
		{
			route.push_back(last);
			last = next[last];
		}
	}
	return res;

}
