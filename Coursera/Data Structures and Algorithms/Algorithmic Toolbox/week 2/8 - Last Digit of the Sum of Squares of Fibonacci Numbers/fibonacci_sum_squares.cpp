#include <iostream>
#include <vector>

int fibonacci_sum_squares_fast(long long n) {
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
    int pissano_squares_sum = 0;
    for (int i = 0; i < pissano_size; ++i) {
        pissano_squares_sum = (pissano_squares_sum + (pissano[i] * pissano[i]) % 10) % 10;
    }
    int period_count = (n + 1) / pissano_size;
    int head_squares_sum = ((period_count % 10) * (pissano_squares_sum)) % 10;

    // tail_sum
    int remainder_count = (n + 1) % pissano_size;
    int tail_squares_sum = 0;
    for (int i = 0; i < remainder_count; ++i) {
        tail_squares_sum = (tail_squares_sum + (pissano[i] * pissano[i]) % 10) % 10;
    }

    return (head_squares_sum + tail_squares_sum) % 10;
}

int main() {
    long long n = 0;
    std::cin >> n;
    std::cout << fibonacci_sum_squares_fast(n);
}
