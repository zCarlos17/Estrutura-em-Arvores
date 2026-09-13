#include "Trie.hpp"
#include <fstream>
using namespace std;
NoTrie::NoTrie(){
	fimDePalavra = false;
	for (int i=0; i<26; i++){
		filhos[i] = nullptr;
	}
}

Trie::Trie(){
	raiz = new NoTrie();
}
Trie::~Trie(){
	liberar(raiz);
}
void Trie::liberar(NoTrie* no){
	if (no == nullptr) return;
	for(int i=0; i < 26;i++){
		liberar(no->filhos[i]);
	}
	delete no;
}
void Trie::inserir(const string& palavra){
	NoTrie* atual = raiz;			//String vazia
	for (int i=0; i<palavra.length(); i++){
		int indice = palavra[i] - 'a';			// Converte letra para numero de 0 a 25( posicao no vetor filhos[26])
		if( atual -> filhos[indice] == nullptr){
			atual->filhos[indice] = new NoTrie();
		}
		atual = atual -> filhos[indice];
	}
	atual->fimDePalavra = true; //Depois de processar todas as letras, marca essse ultimo no como fim de palavra valida
}

bool Trie::buscar(const string& palavra){
	NoTrie* atual = raiz;
	for(int i=0; i<palavra.length();i++){
		int indice = palavra[i] - 'a';
		if(atual -> filhos[indice] == nullptr){
			return false;
		}
		atual = atual->filhos[indice];
	}
	//Percorreu todas ass letras sem cair no return false acima,
	//portanto o caminho exite mas pode ser so um prefixo, entao o resultado real depende de fimdepalavra do ultimo no
	return atual->fimDePalavra;
}
bool Trie::possuiPrefixo(const string& prefixo){
	NoTrie* atual = raiz;
	for(int i=0; i<prefixo.length();i++){
		int indice = prefixo[i] - 'a';
		if(atual -> filhos[indice] == nullptr){
			return false;
		}
		atual = atual->filhos[indice];
	}
	return true;
}

bool Trie::removerAux(NoTrie* no, const string& palavra, int indice){
	//retorna true somente quando o no ficou vazio suficiente para o nivel acima poder apagar
	if(indice== palavra.length()){
		no->fimDePalavra = false; //desmarca como fim de palavra

		//verificação se esse no ainda possui algum filho 
		for(int i=0; i < 26;i++){
			if(no->filhos[i] != nullptr){
				return false;	//tem Filhos logo nao pode ser apagado
			}		
		}
		return true; // nao e mais fim de palavra e sem filhos pode ser apagado	
	}
	int i = palavra[indice] - 'a';
	if (no->filhos[i] ==nullptr){
		return false;
	}
	bool filhoPodeSerApagado = removerAux(no->filhos[i], palavra, indice+1);
	if(filhoPodeSerApagado){
		delete no->filhos[i];
		no->filhos[i] = nullptr;
	}
	//checaremos se este no tambem ficou vazio
	if (no->fimDePalavra){
		return false;  // ainda e fim de outra palavra enta nao pode apagar
	}
	for(int j=0; j < 26;j++){
		if (no->filhos[j] != nullptr){
			return false;
		}
	}
	return true;
}

void Trie::remover(const string& palavra){
	removerAux(raiz, palavra, 0);
}

void Trie::exportarDotAux(NoTrie* no, ofstream& arquivo){
	if (no == nullptr) return;

	// usa o proprio endereco do ponteiro como identificador unico do no
	arquivo << "    node" << no
	        << " [label=\"" << (no->fimDePalavra ? "*" : "") << "\"];\n";

	for (int i = 0; i < 26; i++){
		if (no->filhos[i] != nullptr){
			char letra = 'a' + i;

			arquivo << "    node" << no << " -> node" << no->filhos[i]
			        << " [label=\"" << letra << "\"];\n";

			exportarDotAux(no->filhos[i], arquivo);
		}
	}
}

void Trie::exportarDot(const string& caminhoArquivo){
	ofstream arquivo(caminhoArquivo);
	arquivo <<"digraph Trie{\n";
	exportarDotAux(raiz, arquivo);
	arquivo << "}\n";

}
