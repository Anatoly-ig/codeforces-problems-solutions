#include <bits/stdc++.h>

#define all(x) begin(x), end(x)
using std::cin;
using std::cout;
using ll = unsigned long long;
using std::vector;

const auto ready = []()
{
	cin.tie(0);
	std::ios_base::sync_with_stdio(false);
	return true;
}();

struct seg
{
	int ind, x, y;

	seg()
	{}

	seg(int ind_, int x_, int y_) : ind(ind_), x(x_), y(y_)
	{}
};

int main()
{
	const int MX_SEGS = int(2e6 + 10);
	vector<vector<seg>> vec(MX_SEGS);
	int t;
	cin >> t;
	while (t--)
	{
		int n;
		cin >> n;
		vector<std::pair<int, int>> ans(n);
		vector<int> ms(n);
		int min_diversity = 0;
		std::set<int> groups;
		for(int i = 0; i < n; ++i)
		{
			int a, b, m;
			cin >> a >> b >> m;
			ans[i] = { a, b };
			ms[i] = m;
			int group_num = a + b - m;
			groups.insert(group_num);
			int x = a - std::min(a, m);
			int y = a - (m - std::min(b, m));
			vec[group_num].push_back(seg(i, x, y));
		}

		auto solve_greedy = [&](int group_num)
		{
			int last_point = int(-1e9);
			std::sort(all(vec[group_num]), [](auto a, auto b) 
			{
				if (a.y != b.y) return a.y < b.y;
				return a.x > b.x;
			});
			for (auto &el : vec[group_num])
			{
				if (el.x > last_point)
				{
					last_point = el.y;
					++min_diversity;
				}

				int diff_a = ans[el.ind].first - last_point;
				ans[el.ind].first = diff_a;
				ans[el.ind].second = (ms[el.ind] - diff_a);
			}
		};

		for (auto &el : groups) solve_greedy(el);
		for (auto &el : groups) vec[el].clear();

		cout << min_diversity << "\n";
		for(int i = 0; i < n; ++i) cout << ans[i].first << " " << ans[i].second << "\n";
	}
	return 0;
}