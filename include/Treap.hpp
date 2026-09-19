#ifndef TREAP_HPP
#define TREAP_HPP
#include <string>
#include <fstream>

struct NoTreap {
	int chave;
	int prioridade;
	NoTreap* esquerda;
	NoTreap* direita;

	NoTreap(int valor);
};

class Treap{
	private:
	NoTreap* raiz;
		void liberar(NoTreap* no);
		NoTreap* rotacionarDir(NoTreap* y);
		NoTreap* rotacionarEsq(NoTreap* y);
		NoTreap* inserirAux(NoTreap* no, int chave);
		NoTreap* removerAux(NoTreap* no, int chave);
		bool buscarAux(NoTreap* no, int chave);
		void exportarDotAux(NoTreap* no, std::ofstream& arquivo);
		int contarNosAux(NoTreap* no);
	public:
		Treap();	
		~Treap();	
		void inserir(int chave);
		bool buscar(int chave);
		void remover(int chave);
		void exportarDot(const std::string& caminhoArquivo);
		int contarNos();
};
#endif