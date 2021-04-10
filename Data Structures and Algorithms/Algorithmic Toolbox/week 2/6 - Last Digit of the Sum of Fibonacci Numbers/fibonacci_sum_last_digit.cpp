#include <iostream>
#include <vector>

int fibonacci_sum_fast(long long n) {
    using namespace std;
    
    if (n <= 1) {
        return n;
    }
    
    vector<int> pissano;
    pissano.push_back(0);
    pissano.push_back(1);
    int i = 1;
    
    do {
        i += 1;
        int new_pissano = (pissano[i - 1] + pissano[i - 2]) % 10;
        pissano.push_back(new_pissano);
    } while (pissano[i] != 1 || pissano[i - 1] != 0);

    pissano.pop_back();
    pissano.pop_back();
    const int pissano_size = pissano.size();

    // head_sum
    int pissano_sum = 0;
    for (int i = 0; i < pissano_size; ++i) {
        pissano_sum = (pissano_sum + pissano[i]) % 10;
    }
    int period_count = (n + 1) / pissano_size;
    int head_sum = ((period_count % 10) * (pissano_sum)) % 10;

    // tail_sum
    int remainder_count = (n + 1) % pissano_size;
    int tail_sum = 0;
    for (int i = 0; i < remainder_count; ++i) {
        tail_sum = (tail_sum + pissano[i]) % 10;
    }
    
    return (head_sum + tail_sum) % 10;
}

int main() {
    long long n = 0;
    std::cin >> n;
    std::cout << fibonacci_sum_fast(n);
}
