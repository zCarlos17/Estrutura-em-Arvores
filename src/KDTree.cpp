#include "KDTree.hpp"

#include <iostream>
NoKD::NoKD(double px, double py) {
    x = px;
    y = py;
    esquerda = nullptr;
    direita = nullptr;
}

KDTree::KDTree() { raiz = nullptr; }
KDTree::~KDTree() { liberar(raiz); }

void KDTree::liberar(NoKD* no) {
    if (no == nullptr) return;
    liberar(no->esquerda);
    liberar(no->direita);
    delete no;
}

NoKD* KDTree::inserirAux(NoKD* no, double x, double y, int profundidade) {
    if (no == nullptr) return new NoKD(x, y);

    int eixo = profundidade % 2;
    double valorNo, valorNovo;
    if (eixo == 0) { valorNo = no->x; valorNovo = x; }
    else { valorNo = no->y; valorNovo = y; }

    if (valorNovo < valorNo) {
        no->esquerda = inserirAux(no->esquerda, x, y, profundidade + 1);
    } else {
        no->direita = inserirAux(no->direita, x, y, profundidade + 1);
    }
    return no;
}

void KDTree::inserir(double x, double y) { raiz = inserirAux(raiz, x, y, 0); }

bool KDTree::buscarAux(NoKD* no, double x, double y, int profundidade) {
    if (no == nullptr) return false;
    if (no->x == x && no->y == y) return true;

    int eixo = profundidade % 2;
    double valorNo, valorNovo;
    if (eixo == 0) { valorNo = no->x; valorNovo = x; }
    else { valorNo = no->y; valorNovo = y; }

    if (valorNovo < valorNo) return buscarAux(no->esquerda, x, y, profundidade + 1);
    else return buscarAux(no->direita, x, y, profundidade + 1);
}

bool KDTree::buscar(double x, double y) { return buscarAux(raiz, x, y, 0); }


// ===========================================================================
// REMOCAO
//
// Diferente de uma BST comum, aqui o substituto de um no removido nao pode
// ser "o menor da subarvore direita" em sentido generico: ele precisa ser o
// menor na DIMENSAO DE CORTE daquele nivel. Se o no esta num nivel que corta
// por x, o substituto tem de ser o ponto de menor x -- nao o de menor y.
// ===========================================================================

// devolve, entre tres candidatos (qualquer um podendo ser nulo), o que tem
// o menor valor na dimensao alvo
NoKD* KDTree::minimoEntre(NoKD* a, NoKD* b, NoKD* c, int dimensaoAlvo) {
    NoKD* menor = a;

    if (b != nullptr) {
        if (menor == nullptr) menor = b;
        else {
            double vMenor = (dimensaoAlvo == 0) ? menor->x : menor->y;
            double vB     = (dimensaoAlvo == 0) ? b->x     : b->y;
            if (vB < vMenor) menor = b;
        }
    }

    if (c != nullptr) {
        if (menor == nullptr) menor = c;
        else {
            double vMenor = (dimensaoAlvo == 0) ? menor->x : menor->y;
            double vC     = (dimensaoAlvo == 0) ? c->x     : c->y;
            if (vC < vMenor) menor = c;
        }
    }

    return menor;
}

NoKD* KDTree::encontrarMinimo(NoKD* no, int dimensaoAlvo, int profundidade) {
    if (no == nullptr) return nullptr;

    int eixo = profundidade % 2;

    if (eixo == dimensaoAlvo) {
        // o eixo deste nivel EH a dimensao procurada: todo mundo a direita
        // tem valor >= ao deste no, entao o minimo so pode estar a esquerda
        // (ou ser o proprio no, se nao houver subarvore esquerda)
        if (no->esquerda == nullptr) return no;
        return encontrarMinimo(no->esquerda, dimensaoAlvo, profundidade + 1);
    }

    // o corte deste nivel eh na OUTRA dimensao: ela nao ordena nada em
    // relacao a dimensao procurada, entao os dois lados precisam ser olhados
    NoKD* minEsq = encontrarMinimo(no->esquerda, dimensaoAlvo, profundidade + 1);
    NoKD* minDir = encontrarMinimo(no->direita,  dimensaoAlvo, profundidade + 1);
    return minimoEntre(no, minEsq, minDir, dimensaoAlvo);
}

NoKD* KDTree::removerAux(NoKD* no, double x, double y, int profundidade) {
    if (no == nullptr) return nullptr; // ponto nao existe: nada a fazer

    int eixo = profundidade % 2;

    if (no->x == x && no->y == y) {
        // achamos o no a remover

        if (no->direita != nullptr) {
            // CASO 1: existe subarvore direita.
            // O substituto eh o minimo da DIREITA na dimensao de corte deste
            // nivel. Copiamos as coordenadas dele para ca e removemos o
            // original la embaixo, recursivamente.
            NoKD* substituto = encontrarMinimo(no->direita, eixo, profundidade + 1);
            no->x = substituto->x;
            no->y = substituto->y;
            no->direita = removerAux(no->direita, substituto->x, substituto->y, profundidade + 1);

        } else if (no->esquerda != nullptr) {
            // CASO 2: so existe subarvore esquerda.
            // Nao podemos simplesmente promover o filho esquerdo: buscamos o
            // minimo da ESQUERDA na dimensao de corte, copiamos para ca, e
            // depois movemos a subarvore inteira para a DIREITA.
            // Motivo: apos a substituicao, todo ponto remanescente tem valor
            // >= ao novo valor deste no nessa dimensao -- e a convencao de
            // insercao manda ">=" para a direita.
            NoKD* substituto = encontrarMinimo(no->esquerda, eixo, profundidade + 1);
            no->x = substituto->x;
            no->y = substituto->y;
            no->direita  = removerAux(no->esquerda, substituto->x, substituto->y, profundidade + 1);
            no->esquerda = nullptr;

        } else {
            // CASO 3: folha -- pode ser removida diretamente
            delete no;
            return nullptr;
        }

        return no;
    }

    // ainda nao chegamos no alvo: desce pelo lado indicado pelo eixo do nivel
    double valorNo    = (eixo == 0) ? no->x : no->y;
    double valorAlvo  = (eixo == 0) ? x     : y;

    if (valorAlvo < valorNo) {
        no->esquerda = removerAux(no->esquerda, x, y, profundidade + 1);
    } else {
        no->direita  = removerAux(no->direita,  x, y, profundidade + 1);
    }

    return no;
}

void KDTree::remover(double x, double y) {
    raiz = removerAux(raiz, x, y, 0);
}

void KDTree::exportarDotAux(NoKD* no, std::ofstream& arquivo, int profundidade) {
    if (no == nullptr) return;

    // o rotulo mostra o ponto E o eixo de corte daquele nivel, deixando
    // visivel o particionamento espacial alternado (x, y, x, y ...)
    int eixo = profundidade % 2;
    char nomeEixo = (eixo == 0) ? 'x' : 'y';

    arquivo << "    node" << no << " [label=\"(" << no->x << "," << no->y
            << ")\\ncorte: " << nomeEixo << "\"];\n";

    if (no->esquerda != nullptr) {
        arquivo << "    node" << no << " -> node" << no->esquerda
                << " [label=\"E (" << nomeEixo << " menor)\"];\n";
        exportarDotAux(no->esquerda, arquivo, profundidade + 1);
    }
    if (no->direita != nullptr) {
        arquivo << "    node" << no << " -> node" << no->direita
                << " [label=\"D (" << nomeEixo << " maior/igual)\"];\n";
        exportarDotAux(no->direita, arquivo, profundidade + 1);
    }
}

void KDTree::exportarDot(const std::string& caminhoArquivo) {
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "ERRO: nao foi possivel criar " << caminhoArquivo
                  << " -- a pasta de destino existe?\n";
        return;
    }
    arquivo << "digraph KDTree {\n";
    exportarDotAux(raiz, arquivo, 0);
    arquivo << "}\n";
    arquivo.close();
}

int KDTree::contarNosAux(NoKD* no){
    if (no == nullptr) return 0;
    return 1 + contarNosAux(no->esquerda) + contarNosAux(no->direita);
}

int KDTree::contarNos(){
    return contarNosAux(raiz);
}