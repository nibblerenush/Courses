#include <iostream>
#include <string>
#include <vector>

using namespace std;

static const long long x = 263;
static const long long m1 = 1'000'000'007;
static const long long m2 = 1'000'000'009;
static const long long max_len = 500'000;

long long substr_hash(const vector<long long>& h, long long xl, long long m, int a, int l) {
	long long part1 = h[a + l];
	long long part2 = (xl * h[a]) % m;
	long long part12 = part1 - part2;
	part12 = part12 >= 0 ? part12 : (part12 % m + m);
	return part12 % m;
}

class Solver {
	string s;
	vector<long long> h1, h2;
	vector<long long> cached_l_m1, cached_l_m2;

public:	
	Solver(string s) : s(s) {
		// initialization, precalculation
		h1.assign(s.length() + 1, 0);
		h2.assign(s.length() + 1, 0);
		
		for (int i = 1; i <= (int)s.length(); ++i) {
			h1[i] = (x * h1[i - 1] + s[i - 1]) % m1;
			h2[i] = (x * h2[i - 1] + s[i - 1]) % m2;
		}
		
		cached_l_m1.assign(max_len + 1, 0);
		cached_l_m1[0] = 1;
		
		cached_l_m2.assign(max_len + 1, 0);
		cached_l_m2[0] = 1;

		long long x_mod1 = x % m1;
		long long x_pow1 = x_mod1;

		long long x_mod2 = x % m2;
		long long x_pow2 = x_mod2;
		
		for (long long i = 1; i <= max_len; ++i) {
			cached_l_m1[i] = x_pow1;
			cached_l_m2[i] = x_pow2;
			
			x_pow1 = (x_pow1 * x_mod1) % m1;
			x_pow2 = (x_pow2 * x_mod2) % m2;
		}
	}
	
	bool ask(int a, int b, int l) {
		long long xl1 = cached_l_m1[l];
		long long xl2 = cached_l_m2[l];

		long long ha1 = substr_hash(h1, xl1, m1, a, l);
		long long hb1 = substr_hash(h1, xl1, m1, b, l);

		long long ha2 = substr_hash(h2, xl2, m2, a, l);
		long long hb2 = substr_hash(h2, xl2, m2, b, l);

		return (ha1 == hb1) && (ha2 == hb2);
		//return s.substr(a, l) == s.substr(b, l);
	}
};

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0);

	string s;
	int q;
	cin >> s >> q;
	Solver solver(s);
	for (int i = 0; i < q; i++) {
		int a, b, l;
		cin >> a >> b >> l;
		cout << (solver.ask(a, b, l) ? "Yes\n" : "No\n");
	}
}
