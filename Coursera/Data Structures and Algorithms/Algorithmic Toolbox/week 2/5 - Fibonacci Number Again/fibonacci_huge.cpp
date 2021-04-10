#include <iostream>
#include <vector>

long long get_fibonacci_huge_fast(long long n, long long m) {
    using namespace std;

    if (n <= 1) {
        return n;
    }
    
    vector<long long> pissano;
    pissano.push_back(0);
    pissano.push_back(1);
    int i = 1;
    
    do {
        i += 1;
        long long new_pissano = (pissano[i - 1] + pissano[i - 2]) % m;
        pissano.push_back(new_pissano);
    } while (pissano[i] != 1 || pissano[i - 1] != 0);

    pissano.pop_back();
    pissano.pop_back();
    const int pissano_size = pissano.size();
    
    return pissano[n % pissano_size];
}

int main() {
    long long n, m;
    std::cin >> n >> m;
    std::cout << get_fibonacci_huge_fast(n, m) << '\n';
}
