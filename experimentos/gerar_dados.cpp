// ============================================================================
// gerar_dados.cpp
//
// Gera TODOS os datasets usados nos experimentos (Secao 5).
//
// Datasets de STRING (ja existiam):
//   datasets/{curto,medio,longo}_{n}.txt        -> Trie, Patricia
//
// Datasets NOVOS (para Splay, Treap, KD-Tree):
//   datasets/numerico_aleatorio_{n}.txt          -> Splay, Treap (caso medio)
//   datasets/numerico_ordenado_{n}.txt           -> Splay, Treap (pior caso classico de BST)
//   datasets/pontos_{n}.txt                      -> KD-Tree (pares x y)
//   datasets/splay_base_{n}.txt                  -> conjunto inicial para o teste de acesso enviesado
//   datasets/splay_acesso_enviesado_{n}.txt      -> sequencia de ACESSOS 80/20 (localidade temporal)
//   datasets/splay_acesso_uniforme_{n}.txt       -> sequencia de ACESSOS uniformes (grupo de controle)
//
// Compilar:
//   g++ -std=c++11 experimentos/gerar_dados.cpp -o experimentos/gerar_dados
// Rodar (a partir da pasta experimentos/, para os arquivos carem em datasets/):
//   cd experimentos && ./gerar_dados
// ============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

// ---------------------------------------------------------------------------
// Geradores de STRING (ja existentes)
// ---------------------------------------------------------------------------
string gerarPalavra(int comprimento) {
    string palavra = "";
    for (int i = 0; i < comprimento; i++) {
        char letra = 'a' + (rand() % 26);
        palavra += letra;
    }
    return palavra;
}

void gerarArquivoPalavras(const string& caminho, int quantidade, int comprimento) {
    ofstream arquivo(caminho);
    for (int i = 0; i < quantidade; i++) {
        arquivo << gerarPalavra(comprimento) << "\n";
    }
    arquivo.close();
    cout << "Gerado: " << caminho << " (" << quantidade << " palavras de "
         << comprimento << " letras)\n";
}

// ---------------------------------------------------------------------------
// Geradores NUMERICOS (novos) -- para Splay e Treap
// ---------------------------------------------------------------------------
void gerarNumericoAleatorio(const string& caminho, int quantidade) {
    ofstream arquivo(caminho);
    vector<int> valores;
    for (int i = 0; i < quantidade; i++) valores.push_back(i);

    for (int i = (int)valores.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(valores[i], valores[j]);
    }

    for (int v : valores) arquivo << v << "\n";
    arquivo.close();
    cout << "Gerado: " << caminho << " (" << quantidade << " inteiros aleatorios, unicos)\n";
}

void gerarNumericoOrdenado(const string& caminho, int quantidade) {
    ofstream arquivo(caminho);
    for (int i = 0; i < quantidade; i++) arquivo << i << "\n";
    arquivo.close();
    cout << "Gerado: " << caminho << " (" << quantidade << " inteiros ja ordenados)\n";
}

// ---------------------------------------------------------------------------
// Gerador de PONTOS 2D -- para KD-Tree
// ---------------------------------------------------------------------------
void gerarPontos(const string& caminho, int quantidade) {
    ofstream arquivo(caminho);
    for (int i = 0; i < quantidade; i++) {
        double x = (rand() % 1000000) / 100.0;
        double y = (rand() % 1000000) / 100.0;
        arquivo << x << " " << y << "\n";
    }
    arquivo.close();
    cout << "Gerado: " << caminho << " (" << quantidade << " pontos 2D)\n";
}

// ---------------------------------------------------------------------------
// Geradores de ACESSO -- especificos para provar a vantagem da Splay
// ---------------------------------------------------------------------------
void gerarBase(const string& prefixoCaminho, int tamanhoBase,
                vector<int>& baseSaida, vector<int>& quentesSaida) {
    vector<int> base;
    for (int i = 0; i < tamanhoBase; i++) base.push_back(i);
    for (int i = (int)base.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(base[i], base[j]);
    }

    ofstream arqBase(prefixoCaminho + "_base_" + to_string(tamanhoBase) + ".txt");
    for (int v : base) arqBase << v << "\n";
    arqBase.close();

    int tamanhoQuente = max(1, tamanhoBase / 5); // 20%
    vector<int> quentes(base.begin(), base.begin() + tamanhoQuente);

    baseSaida = base;
    quentesSaida = quentes;
}

void gerarAcessos(const string& prefixoCaminho, const vector<int>& base,
                    const vector<int>& quentes, int quantidadeAcessos) {
    ofstream arqEnviesado(prefixoCaminho + "_acesso_enviesado_" + to_string(quantidadeAcessos) + ".txt");
    for (int i = 0; i < quantidadeAcessos; i++) {
        int chave;
        if (rand() % 100 < 80) {
            chave = quentes[rand() % quentes.size()];
        } else {
            chave = base[rand() % base.size()];
        }
        arqEnviesado << chave << "\n";
    }
    arqEnviesado.close();

    ofstream arqUniforme(prefixoCaminho + "_acesso_uniforme_" + to_string(quantidadeAcessos) + ".txt");
    for (int i = 0; i < quantidadeAcessos; i++) {
        int chave = base[rand() % base.size()];
        arqUniforme << chave << "\n";
    }
    arqUniforme.close();

    cout << "Gerado: " << quantidadeAcessos << " acessos (enviesado 80/20 e uniforme) sobre a base de "
         << base.size() << " chaves\n";
}

// ---------------------------------------------------------------------------
int main() {
    srand(42);

    int tamanhos[] = {10, 1000, 10000, 100000};

    int comprimentoCurto = 3;
    int comprimentoMedio = 8;
    int comprimentoLongo = 20;

    cout << "===== Datasets de STRING (Trie / Patricia) =====\n";
    for (int n : tamanhos) {
        gerarArquivoPalavras("datasets/curto_" + to_string(n) + ".txt", n, comprimentoCurto);
        gerarArquivoPalavras("datasets/medio_" + to_string(n) + ".txt", n, comprimentoMedio);
        gerarArquivoPalavras("datasets/longo_" + to_string(n) + ".txt", n, comprimentoLongo);
    }

    cout << "\n===== Datasets NUMERICOS (Splay / Treap) =====\n";
    for (int n : tamanhos) {
        gerarNumericoAleatorio("datasets/numerico_aleatorio_" + to_string(n) + ".txt", n);
        gerarNumericoOrdenado("datasets/numerico_ordenado_" + to_string(n) + ".txt", n);
    }

    cout << "\n===== Datasets de PONTOS 2D (KD-Tree) =====\n";
    for (int n : tamanhos) {
        gerarPontos("datasets/pontos_" + to_string(n) + ".txt", n);
    }

    cout << "\n===== Datasets de ACESSO (Splay -- localidade temporal) =====\n";
    vector<int> baseSplay, quentesSplay;
    gerarBase("datasets/splay", 1000, baseSplay, quentesSplay);

    int volumesDeAcesso[] = {1000, 10000, 100000};
    for (int qtdAcessos : volumesDeAcesso) {
        gerarAcessos("datasets/splay", baseSplay, quentesSplay, qtdAcessos);
    }

    cout << "\nConcluido.\n";
    return 0;
}