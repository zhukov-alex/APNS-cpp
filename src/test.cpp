#include "../inc/notifier.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    char const* tokenHex = "4efa148eb41f2e7103f21410bf48346c1afa148eb41f2e7103f21410bf48346c";

    Device* d = new Device(tokenHex);

    std::cout << "APNS device test" << std::endl;
    std::cout << strlen(d->_tokenHex) << d->_tokenHex << std::endl;
    std::cout << strlen(d->_tokenBin) << d->_tokenBin << std::endl;

    return 0;
}
