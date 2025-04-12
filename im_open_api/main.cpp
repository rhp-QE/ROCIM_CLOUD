#include <expected>
#include <iostream>
#include <print>

using namespace std;

std::expected<int, std::string> open_api(const std::string &path) {
    if (path == "valid_path") {
        return 42; // Simulated file descriptor
    } else {
        return std::unexpected("Invalid path");
    }
}

int main() {
    auto res = open_api("valid_path_");

    return 0;
}
