#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long MaxPairwiseProduct(const std::vector<int>& numbers) {
    const int size = numbers.size();
    
    int max_idx1 = 0;
    int max_num1 = numbers[max_idx1];

    for (int i = 1; i < size; ++i) {
        if (numbers[i] > max_num1) {
            max_num1 = numbers[i];
            max_idx1 = i;
        }
    }

    int max_idx2 = -1;
    int max_num2 = 0;

    for (int i = 0; i < size; ++i) {
        if ((i != max_idx1) && (max_idx2 == -1 || numbers[i] > max_num2)) {
            max_num2 = numbers[i];
            max_idx2 = i;
        }
    }

    return (long long)max_num1 * max_num2;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> numbers[i];
    }

    std::cout << MaxPairwiseProduct(numbers) << "\n";
    return 0;
}
