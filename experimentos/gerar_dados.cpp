#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

std::string gerarPalavra(int comprimento) {
    std::string palavra = "";
    for (int i = 0; i < comprimento; i++) {
        char letra = 'a' + (rand() % 26);
        palavra += letra;
    }
    return palavra;
}

void gerarArquivo(const std::string& caminho, int quantidade, int comprimento) {
    std::ofstream arquivo(caminho);
    for (int i = 0; i < quantidade; i++) {
        arquivo << gerarPalavra(comprimento) << "\n";
    }
    arquivo.close();
    std::cout << "Gerado: " << caminho << " (" << quantidade << " palavras de "
               << comprimento << " letras)\n";
}

int main() {
    srand(42); // seed FIXA -- importante para reprodutibilidade dos experimentos

    int tamanhos[] = {10, 1000, 10000, 100000};

    int comprimentoCurto = 3;
    int comprimentoMedio = 8;
    int comprimentoLongo = 20;

    for (int n : tamanhos) {
        gerarArquivo("datasets/curto_" + std::to_string(n) + ".txt", n, comprimentoCurto);
        gerarArquivo("datasets/medio_" + std::to_string(n) + ".txt", n, comprimentoMedio);
        gerarArquivo("datasets/longo_" + std::to_string(n) + ".txt", n, comprimentoLongo);
    }

    return 0;
}
