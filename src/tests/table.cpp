#include <blip/buffer/table.hpp>
#include <cassert>
#include <iostream>

void test_initialization() {
    std::cout << "Running test_initialization... ";

    std::string initial_content = "Hello World";
    buffer::PieceTable pt(initial_content);

    assert(pt.getText() == "Hello World");

    std::cout << "PASSED\n";
}

void test_empty_initialization() {
    std::cout << "Running test_empty_initialization... ";

    buffer::PieceTable pt;
    assert(pt.getText() == "");

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "--- Starting Piece Table Tests ---\n";

    test_initialization();
    test_empty_initialization();

    std::cout << "--- All Tests Passed! ---\n";
    return 0;
}
