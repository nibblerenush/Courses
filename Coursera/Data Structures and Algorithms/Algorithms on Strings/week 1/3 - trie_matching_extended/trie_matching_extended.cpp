#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

int const Letters = 4;
int const NA = -1;

struct Node
{
	int next[Letters];

	Node()
	{
		fill(next, next + Letters, NA);
	}

	bool isLeaf() const
	{
		return (next[0] == NA && next[1] == NA && next[2] == NA && next[3] == NA);
	}
};

int letterToIndex(char letter)
{
	switch (letter)
	{
	case 'A': return 0; break;
	case 'C': return 1; break;
	case 'G': return 2; break;
	case 'T': return 3; break;
	default: assert(false); return -1;
	}
}

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(const vector<string>& patterns, std::set<int>& is_ended) {
	trie t;
	// write your code here

	// root node - number is 0
	t.push_back(edges{});

	for (const auto& pattern : patterns) {
		int current_node = 0;
		for (const auto& c : pattern) {
			if (t[current_node].count(c)) {
				// next node
				current_node = t[current_node][c];
			}
			else {
				// new node withous edges
				t.push_back(edges{});

				// new edge from cuurent_node to new node
				t[current_node].emplace(c, t.size() - 1);

				// next node
				current_node = t.size() - 1;
			}
		}
		is_ended.insert(current_node);
	}

	return t;
}

vector <int> solve(const string& text, int n, const vector <string>& patterns)
{
	vector <int> result;
	// write your code here

	// build pattern trie
	std::set<int> is_ended;
	trie pattern_trie = build_trie(patterns, is_ended);

	for (int i = 0; i < static_cast<int>(text.size()); ++i) {
		// node in trie, in begin is root
		int current_node = 0;

		// another counter for text
		int j = i;

		while (true) {
			if (is_ended.count(current_node)) {
				result.push_back(i);
				break;
			}
			else if (j < static_cast<int>(text.size()) && pattern_trie[current_node].count(text[j])) {
				current_node = pattern_trie[current_node][text[j]];
				j++;
			}
			else {
				break;
			}
		}
	}

	return result;
}

int main(void)
{
	string t;
	cin >> t;

	int n;
	cin >> n;

	vector <string> patterns(n);
	for (int i = 0; i < n; i++)
	{
		cin >> patterns[i];
	}

	vector <int> ans;
	ans = solve(t, n, patterns);

	for (int i = 0; i < (int)ans.size(); i++)
	{
		cout << ans[i];
		if (i + 1 < (int)ans.size())
		{
			cout << " ";
		}
		else
		{
			cout << endl;
		}
	}

	return 0;
}
