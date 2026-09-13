#include "Patricia.hpp"
using namespace std;

NoPatricia::NoPatricia(const string& rotuloInicial){		//Construtor
	rotulo = rotuloInicial;
	fimDePalavra= false;
	for (int i = 0; i < 26; i++){
		filhos[i] = nullptr;
	}
}

Patricia::Patricia(){
	raiz = new NoPatricia("");		//Raiz representa string vazia
}
Patricia::~Patricia(){
	liberar(raiz);
}

void Patricia::liberar(NoPatricia* no){
	if(no==nullptr) return;
	for (int i = 0; i < 26; i++){
		liberar(no->filhos[i]);
	}
	delete no;
}

void Patricia::inserir(const string& palavra){
	NoPatricia* atual = raiz;
	string restante = palavra;
	while (true){
		if(restante.empty()){
			atual->fimDePalavra = true;
			return;
		}

		int indice=restante[0]-'a';
		NoPatricia* filho = atual ->filhos[indice];

		//Caso B : Nao existe filho, logo cria um no novo com o restante inteiro 
		if (filho == nullptr){
			NoPatricia* novo = new NoPatricia(restante);
			novo->fimDePalavra = true;
			atual ->filhos[indice] = novo;
			return;
		}
		//Quantidade de caracteres em comum entre o rotulo do filho e o restante 
		int comum = 0;
		while (comum < (int)filho->rotulo.length() &&
			   comum < (int)restante.length() &&
			   filho->rotulo[comum] == restante[comum]){
				comum++;
		}
		
		//Caso A: rotulo base inteiro, logo desce e continua
		if (comum == (int)filho->rotulo.length()){
			atual = filho;
			restante = restante.substr(comum);
			continue;
		}

		//Caso C: se bater em só uma parte divide o nó
		string prefixoComum = filho ->rotulo.substr(0, comum);
		string sufixoAntigo = filho ->rotulo.substr(comum);
		string sufixoNovo = restante.substr(comum);

		NoPatricia* intermediario = new NoPatricia(prefixoComum);
		filho -> rotulo = sufixoAntigo;
		intermediario -> filhos[sufixoAntigo[0] - 'a'] = filho;

		if(sufixoNovo.empty()){
			intermediario->fimDePalavra = true;
		} else {
			NoPatricia* novoNo = new NoPatricia(sufixoNovo);
			novoNo->fimDePalavra = true;
			intermediario->filhos[sufixoNovo[0]-'a'] = novoNo;
		}
		atual->filhos[indice] = intermediario;
		return;
	}
}

bool Patricia::buscar(const string& palavra){
	NoPatricia* atual = raiz;
	string restante = palavra;

	while (true){
		if(restante.empty()){
			return atual->fimDePalavra;
		}
		int indice = restante[0]-'a';
		NoPatricia* filho = atual->filhos[indice];
		
		if(filho == nullptr){
			return false;
		}
		if(restante.substr(0, filho->rotulo.length()) != filho->rotulo){
			return false;
		}
		atual = filho;
		restante = restante.substr(filho->rotulo.length());
	}
}
bool Patricia::removerAux(NoPatricia* no, const string& restante){
	if (restante.empty()){
		no->fimDePalavra= false;
	} else {
		int indice = restante[0] - 'a';
		NoPatricia* filho = no->filhos[indice];
		if ( filho == nullptr){
			return false;
		}
		 if (restante.substr(0, filho->rotulo.length()) != filho->rotulo){
            return false; // palavra nao existe (rotulo nao bate)
        }

        string novoRestante = restante.substr(filho->rotulo.length());
        bool filhoPodeSerApagado = removerAux(filho, novoRestante);

        if (filhoPodeSerApagado){
            delete no->filhos[indice];
            no->filhos[indice] = nullptr;
        } else {

            NoPatricia* f = no->filhos[indice];

            if (f != nullptr && !f->fimDePalavra){
                int contador = 0;
                int unico = -1;
                for (int i = 0; i < 26; i++){
                    if (f->filhos[i] != nullptr){
                        contador++;
                        unico = i;
                    }
                }

                if (contador == 1){
                    NoPatricia* neto = f->filhos[unico];

                    f->rotulo = f->rotulo + neto->rotulo; // concatena os rotulos
                    f->fimDePalavra = neto->fimDePalavra;

                    for (int i = 0; i < 26; i++){
                        f->filhos[i] = neto->filhos[i];
                    }

                    delete neto; // o neto "desaparece", fundido no pai
                }
            }
        }
    }
	if(no->fimDePalavra){
		return false;
	}
	for (int i = 0; i < 26; i++){
        if (no->filhos[i] != nullptr){
            return false;
        }
    }
    return true;
}

void Patricia::remover(const string& palavra){
	removerAux(raiz, palavra);
}

bool Patricia::possuiPrefixo(const string& prefixo){
    NoPatricia* atual = raiz;
    string restante = prefixo;

    while (true){
        if (restante.empty()){
            return true; // caminho existe ate aqui -- nao importa fimDePalavra
        }

        int indice = restante[0] - 'a';
        NoPatricia* filho = atual->filhos[indice];

        if (filho == nullptr){
            return false;
        }

        // so podemos comparar ate o menor dos dois tamanhos
        int tamanhoComparar = (int)restante.length();
        if ((int)filho->rotulo.length() < tamanhoComparar){
            tamanhoComparar = (int)filho->rotulo.length();
        }

        if (restante.substr(0, tamanhoComparar) != filho->rotulo.substr(0, tamanhoComparar)){
            return false; // divergiu antes mesmo de completar a comparacao
        }

        // se o restante inteiro coube dentro do rotulo do filho e bateu,
        // o prefixo eh valido, independente de fimDePalavra
        if ((int)restante.length() <= (int)filho->rotulo.length()){
            return true;
        }

        // restante ainda tem mais caracteres depois do rotulo -- continua descendo
        atual = filho;
        restante = restante.substr(filho->rotulo.length());
    }
}


void Patricia::exportarDotAux(NoPatricia* no, ofstream& arquivo){
	if(no==nullptr) return;
	arquivo << "    node" << no << " [label=\"" << no->rotulo << (no->fimDePalavra ? "*" : "") << "\"];\n";
    for (int i = 0; i < 26; i++) {
		if(no->filhos[i] != nullptr){
			arquivo << "    node" << no << " -> node" << no->filhos[i] << ";\n";
            exportarDotAux(no->filhos[i], arquivo);
		}
	}
}
void Patricia::exportarDot(const string& caminhoArquivo){
	ofstream arquivo(caminhoArquivo);
	arquivo << "digraph Patricia{\n";
	exportarDotAux(raiz, arquivo);
	arquivo << "}\n";
	arquivo.close();
}