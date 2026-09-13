#include "Splay.hpp"
#include <iostream>
using namespace std;

int main() {
    Splay s;
    s.inserir(10);
    s.inserir(5);
    s.inserir(2);

    s.exportarDot("splay_zigzig.dot");

    return 0;
}