#ifndef TRIE_HPP
#define TRIE_HPP
#include <iostream>



struct NoTrie{
	NoTrie* filhos [26];
	bool fimDePalavra;

	NoTrie();  // contstrutor para inicializar os 26 ponteiros como nullptr
};

class Trie {
	private: 
		NoTrie* raiz ;
		void liberar(NoTrie* no);
		bool removerAux(NoTrie* no, const std::string& palavra, int indice);
		void exportarDotAux(NoTrie* no, std::ofstream& arquivo);
	public: 
		Trie();
		~Trie(); // Destrutor  para liberar a memoria recursivamente

	void inserir (const std::string& palavra);
	bool buscar (const std::string& palavra);
	bool possuiPrefixo (const std::string& prefixo);	//Operação especifica da Trie para se autocompletar
	void remover (const std::string& palavra);		

	void exportarDot(const std::string& caminhoArquivo);
};


#endif
