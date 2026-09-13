#include "Patricia.hpp"
#include <iostream>
using namespace std;

void separador(const string& titulo){
    cout << "\n===== " << titulo << " =====\n";
}

int main(){

    // ---------- BLOCO 1: Insercao + busca positiva ----------
    separador("BLOCO 1 - Insercao e busca positiva");
    Patricia p1;

    string palavras[] = {
        "casa", "casal", "casamento", "casamenteiro",
        "casarao", "casaria", "caso", "cachorro",
        "carro", "carta", "cartao"
    };
    int total = 11;

    for (int i = 0; i < total; i++){
        p1.inserir(palavras[i]);
    }

    // Previsao: TODAS devem retornar 1 (true)
    for (int i = 0; i < total; i++){
        cout << "buscar(\"" << palavras[i] << "\") = " << p1.buscar(palavras[i]) << "\n";
    }

    p1.exportarDot("visualizacoes/patricia/patricia_bloco1.dot");


    // ---------- BLOCO 2: Busca negativa (prefixos que nao sao palavras) ----------
    separador("BLOCO 2 - Busca negativa");
    cout << "buscar(\"cas\") = " << p1.buscar("cas") << "\n";
    cout << "buscar(\"car\") = " << p1.buscar("car") << "\n";
    cout << "buscar(\"ca\")  = " << p1.buscar("ca")  << "\n";
    cout << "buscar(\"ment\") = " << p1.buscar("ment") << "\n";


    // ---------- BLOCO 3: possuiPrefixo ----------
    separador("BLOCO 3 - possuiPrefixo");
    cout << "possuiPrefixo(\"cas\") = " << p1.possuiPrefixo("cas") << "\n";
    cout << "possuiPrefixo(\"car\") = " << p1.possuiPrefixo("car") << "\n";
    cout << "possuiPrefixo(\"ca\")  = " << p1.possuiPrefixo("ca")  << "\n";
    cout << "possuiPrefixo(\"cx\")  = " << p1.possuiPrefixo("cx")  << "\n";
    cout << "possuiPrefixo(\"casame\") = " << p1.possuiPrefixo("casame") << "\n";


    // ---------- BLOCO 4: Remocao com fusao (remove "casarao") ----------
    separador("BLOCO 4 - Remocao com fusao (casarao)");
    p1.remover("casarao");

    cout << "buscar(\"casarao\") = " << p1.buscar("casarao") << "\n";
    cout << "buscar(\"casaria\") = " << p1.buscar("casaria") << "\n";
    cout << "buscar(\"casa\") = "     << p1.buscar("casa")     << "\n";
    cout << "buscar(\"casal\") = "    << p1.buscar("casal")    << "\n";
    cout << "buscar(\"casamento\") = " << p1.buscar("casamento") << "\n";

    p1.exportarDot("visualizacoes/patricia/patricia_bloco4_pos_remocao.dot");


    // ---------- BLOCO 5: Remocao de palavra que e prefixo de outra ----------
    separador("BLOCO 5 - Remocao de prefixo (casamento, mantendo casamenteiro)");
    p1.remover("casamento");

    cout << "buscar(\"casamento\") = "     << p1.buscar("casamento")     << "\n";
    cout << "buscar(\"casamenteiro\") = "  << p1.buscar("casamenteiro")  << "\n";

    p1.exportarDot("visualizacoes/patricia/patricia_bloco5_pos_remocao.dot");


    // ---------- BLOCO EXTRA: remover tudo e conferir arvore vazia ----------
    separador("BLOCO EXTRA - remocao total");
    string restantes[] = {"casa", "casal", "casamenteiro", "casaria", "caso",
                           "cachorro", "carro", "carta", "cartao"};
    for (int i = 0; i < 9; i++){
        p1.remover(restantes[i]);
    }
    for (int i = 0; i < 9; i++){
        cout << "buscar(\"" << restantes[i] << "\") apos remocao total = "
             << p1.buscar(restantes[i]) << "\n";
    }

    p1.exportarDot("visualizacoes/patricia/patricia_bloco_final_vazia.dot");

    return 0;
}