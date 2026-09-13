#include "Trie.hpp"
//#include "include"
//#include "include"
//#include "include"
//#include "include"

#include <iostream>
using namespace std;

int main(){
	Trie trie;
	//conjunto de palavras commo exemplo
	trie.inserir("casa");
	trie.inserir("casal");
	trie.inserir("casarao");
	trie.inserir("casamento");

	cout<< "buscar(\"casa\") = " <<trie.buscar("casa")<< "\n";
	cout<< "buscar(\"cas\") = " <<trie.buscar("cas")<< "\n";
	cout<< "buscar(\"casaco\") = " <<trie.buscar("casaco")<< "\n";
	cout<< "possuiPrefixo(\"cas\") = " <<trie.possuiPrefixo("cas")<< "\n";
	
	trie.exportarDot("visualizacoes/trie/trie.dot");

	return 0;
}