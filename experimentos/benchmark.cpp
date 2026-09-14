#include "Trie.hpp"
#include "Patricia.hpp"
#include "Splay.hpp"
#include "Treap.hpp"
#include "KDTree.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

// ============================================================================
// LEITORES DE DATASET
// ============================================================================
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

std::vector<int> lerInteiros(const std::string& caminho) {
    std::vector<int> valores;
    std::ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        std::cout << "ERRO: nao foi possivel abrir " << caminho << "\n";
        return valores;
    }

    int v;
    while (arquivo >> v) {
        valores.push_back(v);
    }
    return valores;
}

std::vector<std::pair<double,double>> lerPontos(const std::string& caminho) {
    std::vector<std::pair<double,double>> pontos;
    std::ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        std::cout << "ERRO: nao foi possivel abrir " << caminho << "\n";
        return pontos;
    }

    double x, y;
    while (arquivo >> x >> y) {
        pontos.push_back({x, y});
    }
    return pontos;
}


// ============================================================================
// BLOCO 1 -- Trie / Patricia (chave string)
// ============================================================================
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


// ============================================================================
// BLOCO 2 -- Splay / Treap (chave int)
// ============================================================================
template <typename Estrutura>
void benchmarkEstruturaNumerica(const std::string& nomeEstrutura, const std::string& nomeCaso,
                                  int tamanho, const std::vector<int>& valores,
                                  std::ofstream& csv, std::ofstream& log) {
    Estrutura estrutura;

    auto inicioInsercao = std::chrono::high_resolution_clock::now();
    for (int v : valores) {
        estrutura.inserir(v);
    }
    auto fimInsercao = std::chrono::high_resolution_clock::now();

    double tempoInsercao = std::chrono::duration<double, std::milli>(fimInsercao - inicioInsercao).count();

    auto inicioBusca = std::chrono::high_resolution_clock::now();
    for (int v : valores) {
        estrutura.buscar(v);
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
void rodarTodosOsTestesNumericos(const std::string& nomeEstrutura, std::ofstream& csv, std::ofstream& log) {
    // "aleatorio" -> caso medio;  "ordenado" -> pior caso classico de BST sem balanceamento
    std::vector<std::string> casos = {"aleatorio", "ordenado"};
    std::vector<int> tamanhos = {10, 1000, 10000, 100000};

    std::string cabecalho = "===== " + nomeEstrutura + " (numerico) =====";
    std::cout << cabecalho << "\n";
    log << cabecalho << "\n";

    for (const std::string& caso : casos) {
        for (int n : tamanhos) {
            std::string caminho = "datasets/numerico_" + caso + "_" + std::to_string(n) + ".txt";
            std::vector<int> valores = lerInteiros(caminho);

            if (valores.empty()) {
                std::string aviso = "AVISO: dataset vazio ou nao encontrado: " + caminho;
                std::cout << aviso << "\n";
                log << aviso << "\n";
                continue;
            }

            benchmarkEstruturaNumerica<Estrutura>(nomeEstrutura, caso, n, valores, csv, log);
        }
    }

    std::cout << "\n";
    log << "\n";
}


// ============================================================================
// BLOCO 3  -- KD-Tree (chave par de doubles)
// ============================================================================
void rodarTestesKDTree(std::ofstream& csv, std::ofstream& log) {
    std::vector<int> tamanhos = {10, 1000, 10000, 100000};

    std::string cabecalho = "===== KDTree =====";
    std::cout << cabecalho << "\n";
    log << cabecalho << "\n";

    for (int n : tamanhos) {
        std::string caminho = "datasets/pontos_" + std::to_string(n) + ".txt";
        std::vector<std::pair<double,double>> pontos = lerPontos(caminho);

        if (pontos.empty()) {
            std::string aviso = "AVISO: dataset vazio ou nao encontrado: " + caminho;
            std::cout << aviso << "\n";
            log << aviso << "\n";
            continue;
        }

        KDTree kd;

        auto inicioInsercao = std::chrono::high_resolution_clock::now();
        for (auto& p : pontos) {
            kd.inserir(p.first, p.second);
        }
        auto fimInsercao = std::chrono::high_resolution_clock::now();
        double tempoInsercao = std::chrono::duration<double, std::milli>(fimInsercao - inicioInsercao).count();

        auto inicioBusca = std::chrono::high_resolution_clock::now();
        for (auto& p : pontos) {
            kd.buscar(p.first, p.second);
        }
        auto fimBusca = std::chrono::high_resolution_clock::now();
        double tempoBusca = std::chrono::duration<double, std::milli>(fimBusca - inicioBusca).count();

        csv << "KDTree,pontos," << n << "," << tempoInsercao << "," << tempoBusca << "\n";

        std::string mensagem = "KDTree | pontos | n=" + std::to_string(n) +
                                " -> insercao=" + std::to_string(tempoInsercao) + "ms, busca=" +
                                std::to_string(tempoBusca) + "ms";
        std::cout << mensagem << "\n";
        log << mensagem << "\n";
    }

    std::cout << "\n";
    log << "\n";
}


// ============================================================================
// BLOCO 4  -- Localidade temporal: Splay vs Treap
//
// Este é o experimento que prova a vantagem teorica da Splay:
// "aproxima elementos frequentemente consultados da raiz".
//
// Metodologia:
//   1. Insere a MESMA base de chaves em uma Splay e em uma Treap.
//   2. Mede o tempo de busca sob duas sequencias de acesso diferentes,
//      geradas pelo gerar_dados.cpp:
//        - ENVIESADA (80/20): 20% das chaves respondem por ~84% dos acessos
//        - UNIFORME: todas as chaves tem a mesma chance
//   3. Compara os 4 resultados (Splay/enviesado, Splay/uniforme,
//      Treap/enviesado, Treap/uniforme).
//
// Expectativa teorica: a Splay deveria ser NOTAVELMENTE mais rapida no
// caso enviesado (porque as chaves quentes ficam perto da raiz), enquanto
// a Treap nao deveria mudar de comportamento entre enviesado e uniforme
// (ja que ela nao se reorganiza por acesso).
// ============================================================================
template <typename Estrutura>
void benchmarkLocalidade(const std::string& nomeEstrutura, const std::string& tipoAcesso,
                           const std::vector<int>& base, const std::vector<int>& acessos,
                           std::ofstream& csv, std::ofstream& log) {
    Estrutura estrutura;
    for (int v : base) {
        estrutura.inserir(v);
    }

    auto inicio = std::chrono::high_resolution_clock::now();
    for (int chave : acessos) {
        estrutura.buscar(chave);
    }
    auto fim = std::chrono::high_resolution_clock::now();

    double tempoTotal = std::chrono::duration<double, std::milli>(fim - inicio).count();
    double tempoMedioPorAcesso = tempoTotal / acessos.size();

    csv << nomeEstrutura << "," << tipoAcesso << "," << base.size() << ","
        << acessos.size() << "," << tempoTotal << "," << tempoMedioPorAcesso << "\n";

    std::string mensagem = nomeEstrutura + " | " + tipoAcesso + " | base=" + std::to_string(base.size()) +
                            " acessos=" + std::to_string(acessos.size()) +
                            " -> total=" + std::to_string(tempoTotal) + "ms, media_por_acesso=" +
                            std::to_string(tempoMedioPorAcesso) + "ms";
    std::cout << mensagem << "\n";
    log << mensagem << "\n";
}

void rodarTestesLocalidade(std::ofstream& csv, std::ofstream& log) {
    std::string cabecalho = "===== LOCALIDADE TEMPORAL (Splay vs Treap) =====";
    std::cout << cabecalho << "\n";
    log << cabecalho << "\n";

    std::vector<int> base = lerInteiros("datasets/splay_base_1000.txt");
    if (base.empty()) {
        std::string aviso = "AVISO: datasets/splay_base_1000.txt nao encontrado -- rode gerar_dados primeiro";
        std::cout << aviso << "\n";
        log << aviso << "\n";
        return;
    }

    std::vector<int> volumesDeAcesso = {1000, 10000, 100000};

    for (int qtd : volumesDeAcesso) {
        std::vector<int> acessoEnviesado = lerInteiros("datasets/splay_acesso_enviesado_" + std::to_string(qtd) + ".txt");
        std::vector<int> acessoUniforme  = lerInteiros("datasets/splay_acesso_uniforme_"  + std::to_string(qtd) + ".txt");

        if (acessoEnviesado.empty() || acessoUniforme.empty()) continue;

        benchmarkLocalidade<Splay>("Splay", "enviesado", base, acessoEnviesado, csv, log);
        benchmarkLocalidade<Splay>("Splay", "uniforme",  base, acessoUniforme,  csv, log);
        benchmarkLocalidade<Treap>("Treap", "enviesado", base, acessoEnviesado, csv, log);
        benchmarkLocalidade<Treap>("Treap", "uniforme",  base, acessoUniforme,  csv, log);
    }

    std::cout << "\n";
    log << "\n";
}


// ============================================================================
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
    rodarTodosOsTestesNumericos<Splay>("Splay", csv, log);
    rodarTodosOsTestesNumericos<Treap>("Treap", csv, log);
    rodarTestesKDTree(csv, log);

    csv.close();
    log.close();

    // arquivo SEPARADO para o experimento de localidade, ja que o formato
    // de colunas eh diferente (tem "tipo_acesso" e "qtd_acessos" em vez de
    // "tempo_insercao"/"tempo_busca" separados)
    std::ofstream csvLocalidade("resultados/localidade_splay.csv");
    std::ofstream logLocalidade("resultados/log_localidade.txt");

    if (!csvLocalidade.is_open() || !logLocalidade.is_open()) {
        std::cout << "ERRO: nao foi possivel criar os arquivos de saida de localidade\n";
        return 1;
    }
    csvLocalidade << "estrutura,tipo_acesso,tamanho_base,qtd_acessos,tempo_total_ms,tempo_medio_por_acesso_ms\n";

    rodarTestesLocalidade(csvLocalidade, logLocalidade);

    csvLocalidade.close();
    logLocalidade.close();

    std::cout << "\nBenchmark concluido!\n";
    std::cout << "Resultados gerais em resultados/tempos.csv e resultados/log.txt\n";
    std::cout << "Resultados de localidade em resultados/localidade_splay.csv e resultados/log_localidade.txt\n";
    return 0;
}