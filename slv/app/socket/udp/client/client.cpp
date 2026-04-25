#include <iostream>
#include <string>

int main() {
    std::string input;

    std::cout << "Nhap chuoi: ";
    std::getline(std::cin, input);

    std::cout << "Ban vua nhap: " << input << std::endl;

    return 0;
}
