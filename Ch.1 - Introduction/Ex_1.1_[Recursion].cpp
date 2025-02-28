#include <vector>
#include <iostream>

std::vector<int> fibonnaci (std::vector<int> fib, int n) {
    if (fib.size() < n) {
        int fib_new = fib[fib.size() - 1] + fib[fib.size() - 2];
        std::cout << "i = " << fib.size() + 1 << " , fib = " << fib_new << std::endl;
        fib.push_back(fib_new);
        fibonnaci(fib, n);
    }

    return fib;
}

int main () {
    std::vector<int> fib = {1,1};
    int n;
    std::cout << "What n to compute fibonnaci?: " << std::endl;
    std::cin >> n;

    fibonnaci(fib, n);

    return 0;
}