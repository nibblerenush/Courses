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

long long get_fibonacci_partial_sum_fast(long long from, long long to) {
    if (from == 0) {
        return fibonacci_sum_fast(to);
    }

    int digit_less = fibonacci_sum_fast(from - 1);
    int digit_greater = fibonacci_sum_fast(to);

    if (digit_greater >= digit_less) {
        return digit_greater - digit_less;
    }
    return (digit_greater + 10) - digit_less;
}

int main() {
    long long from, to;
    std::cin >> from >> to;
    std::cout << get_fibonacci_partial_sum_fast(from, to) << '\n';
}
