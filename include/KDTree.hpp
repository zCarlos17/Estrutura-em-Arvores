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
    void exportarDotAux(NoKD* no, std::ofstream& arquivo);

public:
    KDTree();
    ~KDTree();

    void inserir(double x, double y);
    bool buscar(double x, double y);

    void exportarDot(const std::string& caminhoArquivo);
};

#endif