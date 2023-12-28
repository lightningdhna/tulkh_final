#pragma once
#include <vector>

using namespace std;
class DSU
{
public:
	vector<int> mp;
	vector<int> h;
	vector<int> len;
	DSU(int n)
	{
		mp.resize(n + 1, -1);
		h.resize(n + 1, 1);
		len.resize(n + 1, 0);
	}

	bool can_join(int u, int v) const
	{
		while (mp[u] != -1)
			u = mp[u];
		while (mp[v] != -1)
			v = mp[v];
		return u != v;
	}
	void join(int u, int v)
	{
		while (mp[u] != -1)
			u = mp[u];
		while (mp[v] != -1)
			v = mp[v];
		if (h[u] < h[v])
		{
			mp[u] = v;
		}
		else if (h[u] > h[v])
		{
			mp[v] = u;
		}
		else
		{
			mp[v] = u;
			h[u]++;
		}
	}
	bool try_join(int u, int v)
	{
		while (mp[u] != -1)
			u = mp[u];
		while (mp[v] != -1)
			v = mp[v];
		if (u == v)
			return false;
		if (h[u] < h[v])
		{
			mp[u] = v;
		}
		else if (h[u] > h[v])
		{
			mp[v] = u;
		}
		else
		{
			mp[v] = u;
			h[u]++;
		}
		return true;
	}

	int get_len(int v) const
	{
		while (mp[v] != -1)
			v = mp[v];
		return len[v];
	}
	void change_len(int v, const int value)
	{
		while (mp[v] != -1)
		{
			v = mp[v];
		}
		len[v] = value;
	}

};
