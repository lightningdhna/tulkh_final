#pragma once


#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

class Problem
{
public:

	int vertices_number = 0;
	int postman_number = 0;
	int max_edge = 0;
	vector < vector<int>> distance_matrix;
	void read_input()
	{
		cin >> vertices_number >> postman_number;
		distance_matrix.resize(vertices_number + 5);
		int a;
		for (int i = 0; i <= vertices_number; i++)
		{
			distance_matrix[i].resize(vertices_number + 5, 0);
			for (int j = 0; j <= vertices_number; j++)
			{
				cin >> a;
				distance_matrix[i][j] = a;
				max_edge = max(max_edge, a);
			}
		}
		for (int i = vertices_number + 1; i < distance_matrix.size(); i++)
			distance_matrix[i].resize(vertices_number + 5, 0);
	}
	void read_input(const string& path_url)
	{
		ifstream file(path_url);
		file >> vertices_number >> postman_number;
		distance_matrix.resize(vertices_number + 5);
		for (int i = 0; i <= vertices_number; i++)
		{
			distance_matrix[i].resize(vertices_number + 5, 0);
			for (int j = 0; j <= vertices_number; j++)
			{
				file >> distance_matrix[i][j];
			}
		}
		for (int i = vertices_number + 1; i < distance_matrix.size(); i++)
			distance_matrix[i].resize(vertices_number + 5, 0);
	}


};
