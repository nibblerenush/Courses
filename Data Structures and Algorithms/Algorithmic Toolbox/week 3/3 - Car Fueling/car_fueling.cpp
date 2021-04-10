#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;

int compute_min_refills(int dist, int tank, vector<int> & stops) {
    // write your code here
    const int stops_size = stops.size();
    stops.push_back(dist);
    
    int cur_stop = -1;
    int refill_count = 0;
    
    while (cur_stop <= stops_size) {
        int last_stop = cur_stop;
        while (cur_stop <= stops_size && (stops[cur_stop + 1] - (last_stop == -1 ? 0 : stops[last_stop])) <= tank) {
            cur_stop += 1;
        }

        if (cur_stop == last_stop) {
            return -1;
        }

        if (cur_stop <= stops_size) {
            refill_count += 1;
        }
    }
    
    return refill_count;
}

int main() {
    int d = 0;
    cin >> d;
    int m = 0;
    cin >> m;
    int n = 0;
    cin >> n;

    vector<int> stops(n);
    for (int i = 0; i < n; ++i)
        cin >> stops.at(i);

    cout << compute_min_refills(d, m, stops) << "\n";

    return 0;
}
