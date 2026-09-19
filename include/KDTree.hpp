#ifndef KDTREE_HPP
#define KDTREE_HPP
#include <string>
#include <fstream>

struct NoKD {
    double x, y;
    NoKD* esquerda;
    NoKD* direita;

    NoKD(double px, double py);
};

class KDTree {
private:
    NoKD* raiz;

    void liberar(NoKD* no);
    NoKD* inserirAux(NoKD* no, double x, double y, int profundidade);
    bool buscarAux(NoKD* no, double x, double y, int profundidade);
    void exportarDotAux(NoKD* no, std::ofstream& arquivo, int profundidade);

    NoKD* encontrarMinimo(NoKD* no, int dimensaoAlvo, int profundidade);
    NoKD* minimoEntre(NoKD* a, NoKD* b, NoKD* c, int dimensaoAlvo);
    NoKD* removerAux(NoKD* no, double x, double y, int profundidade);
    int contarNosAux(NoKD* no);

public:
    KDTree();
    ~KDTree();

    void inserir(double x, double y);
    bool buscar(double x, double y);
    void remover(double x, double y);

    void exportarDot(const std::string& caminhoArquivo);
    int contarNos();
};

#endif