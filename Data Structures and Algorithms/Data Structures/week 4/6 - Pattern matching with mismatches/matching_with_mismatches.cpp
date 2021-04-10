#include <iostream>
#include <vector>

using namespace std;

static const long long x = 263;
static const long long m1 = 1'000'000'007;
static const long long m2 = 1'000'000'009;
static const long long max_len = 200'000;

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

void get_mismatch_count(
	const int k,
	const string& text,
	int left_text,
	int right_text,
	const string& pattern,
	int left_pattern,
	int right_pattern,
	const vector<long long>& cached_powers_m1,
	const vector<long long>& cached_powers_m2,
	PrefixHashes& prefix_hashes_t,
	PrefixHashes& prefix_hashes_p,
	int& mismatch_count)
{
	if (left_text <= right_text && left_pattern <= right_pattern && mismatch_count <= k) {
		int mid_text = left_text + (right_text - left_text) / 2;
		int mid_pattern = left_pattern + (right_pattern - left_pattern) / 2;

		if (text[mid_text] != pattern[mid_pattern]) {
			mismatch_count += 1;
		}
		
		int len_left_part = (mid_text - 1) - (left_text) + 1;
		long long ht1 = substr_hash(prefix_hashes_t.h1, cached_powers_m1[len_left_part], m1, left_text, len_left_part);
		long long ht2 = substr_hash(prefix_hashes_t.h2, cached_powers_m2[len_left_part], m2, left_text, len_left_part);
		long long hp1 = substr_hash(prefix_hashes_p.h1, cached_powers_m1[len_left_part], m1, left_pattern, len_left_part);
		long long hp2 = substr_hash(prefix_hashes_p.h2, cached_powers_m2[len_left_part], m2, left_pattern, len_left_part);

		if (ht1 != hp1 || ht2 != hp2) {
			get_mismatch_count(
				k,
				text,
				left_text,
				mid_text - 1,
				pattern,
				left_pattern,
				mid_pattern - 1,
				cached_powers_m1,
				cached_powers_m2,
				prefix_hashes_t,
				prefix_hashes_p,
				mismatch_count);
		}

		int len_right_part = (right_text) - (mid_text + 1) + 1;
		ht1 = substr_hash(prefix_hashes_t.h1, cached_powers_m1[len_right_part], m1, mid_text + 1, len_right_part);
		ht2 = substr_hash(prefix_hashes_t.h2, cached_powers_m2[len_right_part], m2, mid_text + 1, len_right_part);
		hp1 = substr_hash(prefix_hashes_p.h1, cached_powers_m1[len_right_part], m1, mid_pattern + 1, len_right_part);
		hp2 = substr_hash(prefix_hashes_p.h2, cached_powers_m2[len_right_part], m2, mid_pattern + 1, len_right_part);

		if (ht1 != hp1 && ht2 != hp2) {
			get_mismatch_count(
				k,
				text,
				mid_text + 1,
				right_text,
				pattern,
				mid_pattern + 1,
				right_pattern,
				cached_powers_m1,
				cached_powers_m2,
				prefix_hashes_t,
				prefix_hashes_p,
				mismatch_count);
		}
	}
}

vector<int> solve(
	const int k,
	const string &text,
	const string &pattern,
	const vector<long long>& cached_powers_m1,
	const vector<long long>& cached_powers_m2)
{
	PrefixHashes prefix_hashes_t(text);
	PrefixHashes prefix_hashes_p(pattern);

	vector<int> pos;
	for (int i = 0; i < (int)text.length() - (int)pattern.length() + 1; ++i) {
		int mismatch_count = 0;
		get_mismatch_count(
			k,
			text,
			i,
			i + (int)pattern.length() - 1,
			pattern,
			0,
			(int)pattern.length() - 1,
			cached_powers_m1,
			cached_powers_m2,
			prefix_hashes_t,
			prefix_hashes_p,
			mismatch_count);

		if (mismatch_count <= k) {
			pos.push_back(i);
		}
	}

	return pos;
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
	int k;
	string t, p;
	while (cin >> k >> t >> p) {
		auto a = solve(k, t, p, cached_powers_m1, cached_powers_m2);
		cout << a.size();
		for (int x : a)
			cout << " " << x;
		cout << "\n";
	}
}
