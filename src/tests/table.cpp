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

void test_insert_end() {
    std::cout << "Running test_insert_end...";
    buffer::PieceTable pt("Hello");
    pt.insert(5, " World");
    assert(pt.getText() == "Hello World");
    std::cout << "PASSED" << std::endl;
}

void test_insert_middle() {
    std::cout << "Running test_insert_middle...";
    buffer::PieceTable pt("HelloWorld");
    pt.insert(5, " ");
    assert(pt.getText() == "Hello World");
    std::cout << "PASSED" << std::endl;
}

void test_insert_beginning() {
    std::cout << "Running test_insert_beginning...";
    buffer::PieceTable pt("World");
    pt.insert(0, "Hello ");
    assert(pt.getText() == "Hello World");
    std::cout << "PASSED" << std::endl;
}

void test_erase_hole_punch() {
    std::cout << "Running test_erase_hole_punch...";
    buffer::PieceTable pt("Hello World");
    pt.erase(3, 5);
    assert(pt.getText() == "Helrld");
    std::cout << "PASSED" << std::endl;
}

void test_erase_spanning_pieces() {
    std::cout << "Running test_erase_spanning_pieces...";
    buffer::PieceTable pt("Hello");
    pt.insert(5, " Beautiful");
    pt.insert(15, " World");
    pt.erase(4, 13);
    assert(pt.getText() == "Hellorld");
    std::cout << "PASSED" << std::endl;
}

void test_erase_swallow_piece() {
    std::cout << "Running test_erase_swallow_piece...";
    buffer::PieceTable pt("A");
    pt.insert(1, "B");
    pt.insert(2, "C");
    pt.erase(1, 1);
    assert(pt.getText() == "AC");
    std::cout << "PASSED" << std::endl;
}

void test_consecutive_inserts() {
    std::cout << "Running test_consecutive_inserts...";
    buffer::PieceTable pt;
    pt.insert(0, "H");
    pt.insert(1, "e");
    pt.insert(2, "l");
    pt.insert(3, "l");
    pt.insert(4, "o");

    assert(pt.getText() == "Hello");
    assert(pt.getPieceCount() == 1);

    std::cout << "PASSED" << std::endl;
}

int main() {
    std::cout << "--- Starting Piece Table Tests ---\n";

    test_initialization();
    test_empty_initialization();
    test_insert_beginning();
    test_insert_middle();
    test_insert_end();
    test_erase_hole_punch();
    test_erase_spanning_pieces();
    test_erase_swallow_piece();
    test_consecutive_inserts();

    std::cout << "--- All Tests Passed! ---\n";
    return 0;
}
