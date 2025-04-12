#include <expected>
#include <iostream>
#include <print>

using namespace std;

std::expected<int, std::string> open_api_test(const std::string &path) {
    if (path == "valid_path") {
        return 42;
    } else {
        return std::unexpected("Invalid path");
    }
}

int Gcd(int a, int b) { return a + b; }

int main() {
    auto res = open_api_test("valid_path_");

    return 0;
}
