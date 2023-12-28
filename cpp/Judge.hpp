#pragma once
#include "Problem.hpp"


#include<iostream>
#include <vector>

using namespace std;

class Judge
{
public:
	int postman_number = 0;
	vector<vector<int>> distance_matrix;
	vector<vector<int>> routes;

	explicit Judge(const Problem& problem)
	{
		this->distance_matrix = problem.distance_matrix;
		//this->postman_number = problem.postman_number;
	}

	void save_solution(const string& path_url = "")
	{


		ifstream file(path_url);
		file >> postman_number;
		routes.resize(postman_number);
		for (int i = 0; i < postman_number; i++)
		{
			int size;
			file >> size;
			routes[i].resize(size);
			for (int j = 0; j < size; j++)
			{
				file >> routes[i][j];
			}
		}
	}
	void save_solution()
	{


		cin >> postman_number;
		routes.resize(postman_number);
		for (int i = 0; i < postman_number; i++)
		{
			int size;
			cin >> size;
			routes[i].resize(size);
			for (int j = 0; j < size; j++)
			{
				cin >> routes[i][j];
			}
		}
	}
	void print_current_score() const
	{
		cout << cal_score(routes) << endl;
	}

	int cal_route_length(const int route_idx, bool debug = false) const
	{
		if (route_idx > routes.size())
		{
			if (debug)
			{
				cout << "bug index " << route_idx << " out of range of postman number" << endl;
			}
			return -1;
		}
		return cal_route_length(routes[route_idx]);
	}
	int cal_route_length(const vector<int>& route, bool debug = false) const
	{
		int len = 0;
		try
		{
			int prev_pos = 0;
			for (const auto& current_pos : route)
			{
				len += distance_matrix[prev_pos][current_pos];
				prev_pos = current_pos;
			}
		}
		catch (exception e)
		{
			if (debug)
			{
				cout << "bug calculate route lenth " << endl;
				cout << e.what() << endl;
			}
		}
		return len;
	}
	int cal_score(const vector<vector<int>>& routes) const
	{
		int _max = -1;
		try
		{
			for (const auto& route : routes)
			{
				_max = max(_max, cal_route_length(route));
			}
		}
		catch (exception e)
		{
			cout << "bug calculate score " << endl;
			cout << e.what() << endl;
		}
		return _max;
	}

	int cal_input_solution(const string& path_url) const
	{
		ifstream file(path_url);
		vector<vector<int>> routes;

		int postman_number = 0;

		file >> postman_number;
		routes.resize(postman_number);
		for (int i = 0; i < postman_number; i++)
		{
			int size;
			file >> size;
			routes[i].resize(size);
			for (int j = 0; j < size; j++)
			{
				file >> routes[i][j];
			}
		}

		return cal_score(routes);

	}
	int cal_input_solution() const
	{
		vector<vector<int>> routes;

		int postman_number = 0;

		cin >> postman_number;
		routes.resize(postman_number);
		for (int i = 0; i < postman_number; i++)
		{
			int size;
			cin >> size;
			routes[i].resize(size);
			for (int j = 0; j < size; j++)
			{
				cin >> routes[i][j];
			}
		}

		return cal_score(routes);

	}
	void print_input_solution(const string& path_url = "") const
	{
		if (path_url.empty())
			cout << cal_input_solution();
		else
			cout << cal_input_solution(path_url) << endl;
	}
};

