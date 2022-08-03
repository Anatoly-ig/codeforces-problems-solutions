#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::pair;
using ll = unsigned long long;

const auto ready = []()
{
	cin.tie(0);
	std::ios_base::sync_with_stdio(false);
	return true;
}();

void solve()
{
	int n, m, k;
	cin >> n >> m >> k;

	vector<vector<pair<int, int>>> adj(n + 1);
	for (int i = 0; i < m; ++i)
	{
		int u, v;
		cin >> u >> v;
		adj[u].push_back({ v, i });
		adj[v].push_back({ u, i });
	}

	vector<int> dist(n + 1, 0), used(n + 1, 0);
	std::queue<int> q;
	q.push(1);
	used[1] = 1;

	while (q.size())
	{
		int cur = q.front();
		q.pop();

		for (auto &[to, i] : adj[cur])
		{
			if (!used[to])
			{
				used[to] = 1;
				dist[to] = dist[cur] + 1;
				q.push(to);
			}
		}
	}

	vector<vector<int>> possibleEdges(n + 1);

	for (int u = 1; u <= n; ++u)
	{
		for (auto &[v, ind] : adj[u])
		{
			if (dist[u] + 1 == dist[v]) possibleEdges[v].push_back(ind);
		}
	}

	int cnt = 0;
	vector<vector<int>> ans;
	vector<int> cur_edges(m, 0);
	
	auto print_ans = [&]()
	{
	    cout << ans.size() << "\n";
	    for(auto &el : ans)
	    {
	        for(auto &k : el) cout << k;
	        cout << "\n";
	    }
	    std::exit(0);
	};
	
	std::function<void(int)> getAnswer = [&](int u)
	{
		if (u == n + 1)
		{
			ans.push_back(cur_edges);
			++cnt;
			if (cnt == k)
			{
				print_ans();
			}
			return;
		}
		for (int i = 0; i < possibleEdges[u].size(); ++i)
		{
			int ind = possibleEdges[u][i];
			cur_edges[ind] = 1;
			getAnswer(u + 1);
			cur_edges[ind] = 0;
		}
	};
	getAnswer(2);
	print_ans();
}

int main()
{
	solve();
	return 0;
}