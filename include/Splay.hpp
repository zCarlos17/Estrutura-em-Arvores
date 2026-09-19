#ifndef SPLAY_HPP
#define SPLAY_HPP
#include <fstream>
#include <string>

struct NoSplay{
	int chave;
	NoSplay* esquerda;
	NoSplay* direita;
	NoSplay* pai;
	NoSplay (int valor);
};

class Splay{
	private:
		NoSplay* raiz;
		void liberar(NoSplay* no);
		void rotacionarEsq(NoSplay* no);
		void rotacionarDir(NoSplay* no);
		void splay(NoSplay* no);
		void exportarDotAux(NoSplay* no, std::ofstream& arquivo);
		int contarNosAux(NoSplay* no);
	public:
		Splay();
		~Splay();

		void inserir(int chave);
		bool buscar(int chave);
		void remover(int chave);
		void exportarDot(const std::string& caminhoArquivo);
		int contarNos();
};
#endif