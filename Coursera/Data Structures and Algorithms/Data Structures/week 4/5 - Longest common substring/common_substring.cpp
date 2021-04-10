#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

static const long long x = 263;
static const long long m1 = 1'000'000'007;
static const long long m2 = 1'000'000'009;
static const long long max_len = 100'000;

long long substr_hash(const vector<long long>& h, long long xl, long long m, int a, int l) {
	long long part1 = h[a + l];
	long long part2 = (xl * h[a]) % m;
	long long part12 = part1 - part2;
	part12 = part12 >= 0 ? part12 : (part12 % m + m);
	return part12 % m;
}

class PrefixHashes {
public:
	const string& s;
	vector<long long> h1, h2;

public:
	PrefixHashes(const string& s_) : s(s_) {
		h1.assign(s.length() + 1, 0);
		h2.assign(s.length() + 1, 0);
		
		for (int i = 1; i <= (int)s.length(); ++i) {
			h1[i] = (x * h1[i - 1] + s[i - 1]) % m1;
			h2[i] = (x * h2[i - 1] + s[i - 1]) % m2;
		}
	}
};

struct Answer {
	size_t i, j, len;
};

Answer solve(
	const string &s,
	const string &t,
	const vector<long long>& cached_powers_m1,
	const vector<long long>& cached_powers_m2)
{
	PrefixHashes prefix_hashes_s(s);
	PrefixHashes prefix_hashes_t(t);

	int left = 0;
	int right = std::min(s.length(), t.length());
	Answer ans = { 0, 0, 0 };

	while (left <= right) {
		int k = left + (right - left) / 2;
		
		long long xl1 = cached_powers_m1[k];
		long long xl2 = cached_powers_m2[k];
		
		unordered_map<long long, int> hs1, hs2;
		for (int i = 0; i < (int)s.length() - k + 1; ++i) {
			hs1[substr_hash(prefix_hashes_s.h1, xl1, m1, i, k)] = i;
			hs2[substr_hash(prefix_hashes_s.h2, xl2, m2, i, k)] = i;
		}
		
		bool is_found = false;
		for (int i = 0; i < (int)t.length() - k + 1; ++i) {
			long long ht1 = substr_hash(prefix_hashes_t.h1, xl1, m1, i, k);
			long long ht2 = substr_hash(prefix_hashes_t.h2, xl2, m2, i, k);
			
			if (hs1.count(ht1) && hs2.count(ht2)) {
				is_found = true;
				ans.i = hs1[ht1];
				ans.j = i;
				ans.len = k;
				break;
			}
		}
		
		if (is_found) {
			left = k + 1;
		}
		else {
			right = k - 1;
		}
	}

	/*for (size_t i = 0; i < s.size(); i++)
		for (size_t j = 0; j < t.size(); j++)
			for (size_t len = 0; i + len <= s.size() && j + len <= t.size(); len++)
				if (len > ans.len && s.substr(i, len) == t.substr(j, len))
					ans = {i, j, len};*/

	return ans;
}

vector<long long> get_cached_powers(long long m) {
	vector<long long> cached_powers;
	cached_powers.assign(max_len + 1, 0);
	cached_powers[0] = 1;
	
	long long x_mod = x % m;
	long long x_pow = x_mod;
	
	for (long long i = 1; i <= max_len; ++i) {
		cached_powers[i] = x_pow;
		x_pow = (x_pow * x_mod) % m;
	}
	
	return cached_powers;
}

int main() {
	vector<long long> cached_powers_m1 = get_cached_powers(m1);
	vector<long long> cached_powers_m2 = get_cached_powers(m2);
	
	ios_base::sync_with_stdio(false), cin.tie(0);
	string s, t;
	while (cin >> s >> t) {
		auto ans = solve(s, t, cached_powers_m1, cached_powers_m2);
		cout << ans.i << " " << ans.j << " " << ans.len << "\n";
	}

	return 0;
}
