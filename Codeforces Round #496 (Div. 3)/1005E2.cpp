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

class segmentTree
{
public:
	vector<int> data;
	int n;

	segmentTree(int n_)
	{
		n = n_;
		data.resize(4 * n);
	}

	void update(int v, int l, int r, int pos, int val)
	{
		if (l == r)
		{
			data[v] += val;
		}
		else
		{
			int mid = (l + r) >> 1;
			if (pos <= mid) update(2 * v + 1, l, mid, pos, val);
			else update(2 * v + 2, mid + 1, r, pos, val);
			data[v] = data[2 * v + 1] + data[2 * v + 2];
		}
	}

	int get(int v, int l, int r, int ql, int qr)
	{
		if (ql > qr) return 0;
		else if (l == ql && r == qr) return data[v];
		else
		{
			int mid = (l + r) >> 1;
			return get(2 * v + 1, l, mid, ql, std::min(mid, qr)) + get(2 * v + 2, mid + 1, r, std::max(mid + 1, ql), qr);
		}
	}

	void update(int pos, int val)
	{
		update(0, 0, n - 1, pos, val);
	}

	int get(int ql, int qr)
	{
		return get(0, 0, n - 1, ql, qr);
	}
};

const int MX_LEN = int(2e6);
const int HALF_LEN = MX_LEN / 2;
int n, median;
ll ans = 0LL;
// b = larger_median - smaller_median
// m = count_median
segmentTree tree_a(MX_LEN);	// m + b
segmentTree tree_b(MX_LEN);	// m - b

void solve(vector<int>& vec, int ql, int qr)
{
	if (ql == qr)
	{
		if (vec[ql] == median) ++ans;
		return;
	}

	int mid = (ql + qr) >> 1;

	int left_len = mid - ql + 1;
	int right_len = qr - (mid + 1) + 1;

	auto get_statistics = [&](vector<std::pair<int, int>>& v, int left, int right, int step)
	{
		int balance = 0, median_count = 0;
		for (int i = left, k = 0; step * i <= step * right; i += step, ++k)
		{
			if (vec[i] > median) ++balance;
			if (vec[i] == median) ++median_count;
			if (vec[i] < median) --balance;
			v[k] = { balance, median_count };
		}
	};

	vector<std::pair<int, int>> qrs(left_len), adds(right_len);
	get_statistics(qrs, mid, ql, -1);
	get_statistics(adds, mid + 1, qr, 1);

	std::sort(all(qrs), [](auto a, auto b) {
		return -a.first < -b.first;
		});

	std::sort(all(adds), [](auto a, auto b) {
		return a.first < b.first;
		});

	vector<int> logs_a, logs_b;

	int ptr = 0;

	for (int i = 0; i < left_len; ++i)
	{
		int pref = -qrs[i].first;
		while (ptr < right_len && adds[ptr].first < pref)
		{
			tree_a.update(adds[ptr].first + adds[ptr].second + HALF_LEN, 1);
			logs_a.push_back(adds[ptr].first + adds[ptr].second + HALF_LEN);
			++ptr;
		}

		ans += tree_a.get(-(qrs[i].first + qrs[i].second) + 1 + HALF_LEN, MX_LEN - 1);
	}

	ptr = 0;
	int cnt_has_median = 0;
	int cnt_without_median = 0;
	for (int i = 0; i < left_len; ++i)
	{
		int val = -qrs[i].first;
		int m_count = qrs[i].second;
		while (ptr < right_len && adds[ptr].first < val) ++ptr;
		if (ptr < right_len && adds[ptr].first == val) cnt_has_median = 0, cnt_without_median = 0;
		if (ptr == right_len && adds.back().first < val) cnt_has_median = 0, cnt_without_median = 0;
		while (ptr < right_len && adds[ptr].first == val)
		{
			if (adds[ptr].second > 0) ++cnt_has_median;
			else ++cnt_without_median;
			++ptr;
		}
		if (m_count > 0) ans += (cnt_has_median + cnt_without_median);
		else ans += cnt_has_median;
	}

	ptr = right_len - 1;
	for (int i = left_len - 1; i >= 0; --i)
	{
		int suff = -qrs[i].first;
		while (ptr >= 0 && adds[ptr].first > suff)
		{
			tree_b.update(adds[ptr].second - adds[ptr].first + HALF_LEN, 1);
			logs_b.push_back(adds[ptr].second - adds[ptr].first + HALF_LEN);
			--ptr;
		}

		ans += tree_b.get(-(qrs[i].second - qrs[i].first) + HALF_LEN, MX_LEN - 1);
	}
	for (auto &el : logs_a) tree_a.update(el, -1);
	for (auto &el : logs_b) tree_b.update(el, -1);
	solve(vec, ql, mid);
	solve(vec, mid + 1, qr);
}

int main()
{
	cin >> n >> median;
	vector<int> vec(n);
	for (int i = 0; i < n; ++i) cin >> vec[i];
	solve(vec, 0, n - 1);
	cout << ans << "\n";
	return 0;
}