#include "Splay.hpp"
using namespace std;

NoSplay::NoSplay(int valor ){			//Construtor do no
	chave = valor;
	esquerda = nullptr;
	direita = nullptr;
	pai = nullptr;
}
//Construtor /Destrutor da Splay
Splay::Splay(){
	raiz = nullptr;
}

Splay::~Splay(){
	liberar(raiz);
}
void Splay::liberar(NoSplay* no){
	if(no==nullptr) return;
	liberar(no->esquerda);
	liberar(no->direita);
	delete no;
}
//-------------------------------

void Splay::rotacionarDir(NoSplay* y){
	NoSplay* x = y->esquerda;
	y->esquerda = x->direita;
	if(x->direita != nullptr){
		x->direita->pai = y;
	}
	x->pai = y->pai;
	if(y->pai == nullptr){
		raiz = x;
	} else if(y == y->pai->esquerda){
		y->pai->esquerda = x;
	} else{
		y->pai->direita = x;
	}

	x->direita = y;
	y->pai = x;
}
void Splay::rotacionarEsq(NoSplay* y){
	NoSplay* x = y->direita;
	y->direita = x->esquerda;
	if(x->esquerda != nullptr){
		x->esquerda->pai = y;
	}
	x->pai = y->pai;
	if(y->pai == nullptr){
		raiz = x;
	} else if(y == y->pai->esquerda){
		y->pai->esquerda = x;
	} else{
		y->pai->direita = x;
	}

	x->esquerda = y;
	y->pai = x;
}

void Splay::splay(NoSplay* x){
	while (x->pai != nullptr){
		NoSplay* pai = x->pai;
		NoSplay* avo = pai->pai;
		
		if (avo == nullptr){ //Zig
			if(x == pai->esquerda) rotacionarDir(pai);
			else rotacionarEsq(pai);	
		}
		else if(x == pai->esquerda && pai == avo->esquerda){	// Esq Esq
			rotacionarDir(avo);
			rotacionarDir(pai);
		}
		else if(x == pai->direita && pai == avo->direita){	//Dir Dir
			rotacionarEsq(avo);
			rotacionarEsq(pai);
		}
		else if(x == pai->direita && pai == avo->esquerda){	// Esq Dir
			rotacionarEsq(pai);
			rotacionarDir(avo);
		}
		else { 	// Dir Esq
			rotacionarDir(pai);
			rotacionarEsq(avo);
		}	
	}	
}

void Splay::inserir(int chave){
	if ( raiz == nullptr){
		raiz = new NoSplay(chave);
		return;
	}
	NoSplay* atual = raiz;
	NoSplay* pai = nullptr;
	while (atual != nullptr) {
        pai = atual;
        if (chave < atual->chave) atual = atual->esquerda;
        else if (chave > atual->chave) atual = atual->direita;
        else { splay(atual); return; }
    }

    NoSplay* novo = new NoSplay(chave);
    novo->pai = pai;
    if (chave < pai->chave) pai->esquerda = novo;
    else pai->direita = novo;

    splay(novo);
}

bool Splay::buscar(int chave) {
    NoSplay* atual = raiz;

    while (atual != nullptr) {
        if (chave == atual->chave) {
            splay(atual);
            return true;
        } else if (chave < atual->chave) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }

    return false;
}
void Splay::remover(int chave) {
    NoSplay* atual = raiz;
    while (atual != nullptr && atual->chave != chave) {
        if (chave < atual->chave) atual = atual->esquerda;
        else atual = atual->direita;
    }

    if (atual == nullptr) return;

    splay(atual);

    if (raiz->esquerda == nullptr) {
        NoSplay* antigo = raiz;
        raiz = raiz->direita;
        if (raiz != nullptr) raiz->pai = nullptr;
        delete antigo;
    } else {
        NoSplay* subEsquerda = raiz->esquerda;
        NoSplay* subDireita = raiz->direita;
        subEsquerda->pai = nullptr;

        NoSplay* antigo = raiz;
        delete antigo;

        NoSplay* maiorEsquerda = subEsquerda;
        while (maiorEsquerda->direita != nullptr) {
            maiorEsquerda = maiorEsquerda->direita;
        }

        raiz = subEsquerda;
        splay(maiorEsquerda);

        raiz->direita = subDireita;
        if (subDireita != nullptr) {
            subDireita->pai = raiz;
        }
    }
}
void Splay::exportarDotAux(NoSplay* no, std::ofstream& arquivo) {
    if (no == nullptr) return;

    // destaca a raiz, ja que na Splay o elemento mais recentemente
    // acessado deveria estar sempre ali -- esse e o ponto central da estrutura
    if (no == raiz) {
        arquivo << "    node" << no << " [label=\"" << no->chave
                << "\", style=filled, fillcolor=lightgray];\n";
    } else {
        arquivo << "    node" << no << " [label=\"" << no->chave << "\"];\n";
    }

    if (no->esquerda != nullptr) {
        arquivo << "    node" << no << " -> node" << no->esquerda << " [label=\"E\"];\n";
        exportarDotAux(no->esquerda, arquivo);
    }
    if (no->direita != nullptr) {
        arquivo << "    node" << no << " -> node" << no->direita << " [label=\"D\"];\n";
        exportarDotAux(no->direita, arquivo);
    }
}

void Splay::exportarDot(const std::string& caminhoArquivo) {
    std::ofstream arquivo(caminhoArquivo);
    arquivo << "digraph Splay {\n";
    exportarDotAux(raiz, arquivo);
    arquivo << "}\n";
    arquivo.close();
}