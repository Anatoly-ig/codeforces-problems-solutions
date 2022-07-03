#include <bits/stdc++.h>

using std::cin;
using std::cout;
using ll = long long;

const auto ready = []()
{
	cin.tie(0);
	std::ios_base::sync_with_stdio(false);
	return true;
}();

void solve()
{
    ll x, y;
    cin >> x >> y;
    if(x > y)
    {
        cout << x + y << "\n";
    }
    else if(x == y)
    {
        cout << x << "\n";
    }
    else
    {
        cout << y - (y % x) / 2 << "\n";
    }
}

int main()
{
    int t;
    cin >> t;
	while(t--) solve();
	return 0;
}