#include "Trie.hpp"
#include "Patricia.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

std::vector<std::string> lerPalavras(const std::string& caminho) {
    std::vector<std::string> palavras;
    std::ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        std::cout << "ERRO: nao foi possivel abrir " << caminho << "\n";
        return palavras;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        if (!linha.empty()) {
            palavras.push_back(linha);
        }
    }
    return palavras;
}

template <typename Estrutura>
void benchmarkEstrutura(const std::string& nomeEstrutura, const std::string& nomeCaso,
                          int tamanho, const std::vector<std::string>& palavras,
                          std::ofstream& csv, std::ofstream& log) {
    Estrutura estrutura;

    auto inicioInsercao = std::chrono::high_resolution_clock::now();
    for (const std::string& palavra : palavras) {
        estrutura.inserir(palavra);
    }
    auto fimInsercao = std::chrono::high_resolution_clock::now();

    double tempoInsercao = std::chrono::duration<double, std::milli>(fimInsercao - inicioInsercao).count();

    auto inicioBusca = std::chrono::high_resolution_clock::now();
    for (const std::string& palavra : palavras) {
        estrutura.buscar(palavra);
    }
    auto fimBusca = std::chrono::high_resolution_clock::now();

    double tempoBusca = std::chrono::duration<double, std::milli>(fimBusca - inicioBusca).count();

    csv << nomeEstrutura << "," << nomeCaso << "," << tamanho << ","
        << tempoInsercao << "," << tempoBusca << "\n";

    std::string mensagem = nomeEstrutura + " | " + nomeCaso + " | n=" + std::to_string(tamanho) +
                            " -> insercao=" + std::to_string(tempoInsercao) + "ms, busca=" +
                            std::to_string(tempoBusca) + "ms";

    std::cout << mensagem << "\n";
    log << mensagem << "\n";
}

template <typename Estrutura>
void rodarTodosOsTestes(const std::string& nomeEstrutura, std::ofstream& csv, std::ofstream& log) {
    std::vector<std::string> casos = {"curto", "medio", "longo"};
    std::vector<int> tamanhos = {10, 1000, 10000, 100000};

    std::string cabecalho = "===== " + nomeEstrutura + " =====";
    std::cout << cabecalho << "\n";
    log << cabecalho << "\n";

    for (const std::string& caso : casos) {
        for (int n : tamanhos) {
            std::string caminho = "datasets/" + caso + "_" + std::to_string(n) + ".txt";
            std::vector<std::string> palavras = lerPalavras(caminho);

            if (palavras.empty()) {
                std::string aviso = "AVISO: dataset vazio ou nao encontrado: " + caminho;
                std::cout << aviso << "\n";
                log << aviso << "\n";
                continue;
            }

            benchmarkEstrutura<Estrutura>(nomeEstrutura, caso, n, palavras, csv, log);
        }
    }

    std::cout << "\n";
    log << "\n";
}

int main() {
    std::ofstream csv("resultados/tempos.csv");
    std::ofstream log("resultados/log.txt");

    if (!csv.is_open() || !log.is_open()) {
        std::cout << "ERRO: nao foi possivel criar os arquivos de saida em resultados/\n";
        return 1;
    }
    csv << "estrutura,caso,tamanho,tempo_insercao_ms,tempo_busca_ms\n";

    rodarTodosOsTestes<Trie>("Trie", csv, log);
    rodarTodosOsTestes<Patricia>("Patricia", csv, log);

    csv.close();
    log.close();
    std::cout << "\nBenchmark concluido! Resultados em resultados/tempos.csv e resultados/log.txt\n";
    return 0;
}