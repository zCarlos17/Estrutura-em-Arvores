#ifndef PATRICIA_HPP
#define PATRICIA_HPP
#include <string>
#include <fstream>

struct NoPatricia{
	std::string rotulo;
	NoPatricia* filhos[26];
	bool fimDePalavra;

	NoPatricia(const std::string& rotuloInicial);
};

class Patricia{
	private:
		NoPatricia* raiz;
		void liberar(NoPatricia* no);
		void exportarDotAux(NoPatricia* no, std::ofstream& arquivo);
		bool removerAux(NoPatricia* no, const std::string& restante);
	public:
		Patricia();
		~Patricia();
		void inserir(const std::string& palavra);
		bool buscar(const std::string& palavra);
		bool possuiPrefixo(const std::string& prefixo);
		void remover (const std::string& palavra);		
		void exportarDot(const std::string& caminhoArquivo);
};

#endif