#include "Trie.hpp"
#include "Patricia.hpp"
#include "Splay.hpp"
#include "Treap.hpp"
#include "KDTree.hpp"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// ---------------------------------------------------------------------------
// Utilitarios de teste: em vez de so imprimir 0/1 e o programador conferir na
// mao, cada teste declara o resultado ESPERADO e o programa avisa na hora se
// bateu ou nao. Isso escala melhor conforme o numero de casos cresce.
// ---------------------------------------------------------------------------
int totalTestes = 0;
int testesFalhos = 0;

void checar(const string& descricao, bool obtido, bool esperado){
    totalTestes++;
    if (obtido == esperado){
        cout << "  [OK] " << descricao << "\n";
    } else {
        testesFalhos++;
        cout << "  [FALHOU] " << descricao
             << " -- esperado=" << esperado << " obtido=" << obtido << "\n";
    }
}

void separador(const string& titulo){
    cout << "\n===== " << titulo << " =====\n";
}


// ============================================================================
// BLOCO 1 -- TRIE
// ============================================================================
void testarTrie(){
    separador("TRIE");
    Trie t;

    string palavras[] = {"casa", "casal", "casamento", "casarao"};
    for (const string& p : palavras) t.inserir(p);

    checar("buscar(\"casa\") apos insercao",       t.buscar("casa"),       true);
    checar("buscar(\"casal\") apos insercao",      t.buscar("casal"),      true);
    checar("buscar(\"cas\") -- so prefixo, nao palavra", t.buscar("cas"),  false);
    checar("possuiPrefixo(\"cas\")",               t.possuiPrefixo("cas"), true);
    checar("possuiPrefixo(\"xy\") -- nao existe",  t.possuiPrefixo("xy"),  false);

    t.remover("casa");
    checar("apos remover \"casa\": buscar(\"casa\")",   t.buscar("casa"),   false);
    checar("apos remover \"casa\": buscar(\"casal\") sobrevive", t.buscar("casal"), true);

    checar("buscar(\"\") em trie nao vazia",       t.buscar(""),           false);
    t.remover("naoexiste");
    checar("apos remover chave inexistente: buscar(\"casal\") intacto", t.buscar("casal"), true);

    t.exportarDot("visualizacoes/trie/trie_teste_funcional.dot");
}


// ============================================================================
// BLOCO 2 -- PATRICIA
// ============================================================================
void testarPatricia(){
    separador("PATRICIA");
    Patricia p;

    string palavras[] = {"casa", "casamento", "casamenteiro", "casarao", "casaria"};
    for (const string& s : palavras) p.inserir(s);

    checar("buscar(\"casamenteiro\")",             p.buscar("casamenteiro"), true);
    checar("buscar(\"casame\") -- so prefixo",     p.buscar("casame"),       false);
    checar("possuiPrefixo(\"casame\") -- termina no meio de um rotulo comprimido",
           p.possuiPrefixo("casame"), true);

    p.remover("casarao");
    checar("apos remover \"casarao\": buscar(\"casarao\")",        p.buscar("casarao"), false);
    checar("apos remover \"casarao\": buscar(\"casaria\") sobrevive (no fundido)", p.buscar("casaria"), true);

    p.remover("casamento");
    checar("apos remover \"casamento\": buscar(\"casamento\")",       p.buscar("casamento"), false);
    checar("apos remover \"casamento\": buscar(\"casamenteiro\") sobrevive", p.buscar("casamenteiro"), true);

    Patricia vazia;
    checar("buscar em Patricia vazia",             vazia.buscar("qualquer"), false);
    checar("possuiPrefixo em Patricia vazia",      vazia.possuiPrefixo("a"), false);

    p.exportarDot("visualizacoes/patricia/patricia_teste_funcional.dot");
}


// ============================================================================
// BLOCO 3 -- SPLAY
// ============================================================================
void testarSplay(){
    separador("SPLAY");
    Splay s;

    checar("buscar em Splay vazia",   s.buscar(10), false);
    s.remover(10);

    int valores[] = {50, 30, 70, 20, 40, 60, 80};
    for (int v : valores) s.inserir(v);

    checar("buscar(40) presente",     s.buscar(40), true);
    checar("buscar(999) ausente",     s.buscar(999), false);

    s.buscar(40);
    checar("busca repetida apos splay nao corrompe a arvore", s.buscar(40), true);

    s.remover(40);
    checar("apos remover raiz(40): buscar(40)",   s.buscar(40),   false);
    checar("apos remover raiz(40): buscar(20) sobrevive", s.buscar(20), true);
    checar("apos remover raiz(40): buscar(80) sobrevive", s.buscar(80), true);

    int restantes[] = {20, 30, 50, 60, 70, 80};
    for (int v : restantes) s.remover(v);
    checar("apos remover tudo: buscar(20)",       s.buscar(20),  false);
    checar("apos remover tudo: buscar(80)",       s.buscar(80),  false);

    s.remover(999);
    checar("remover de arvore ja vazia nao trava", true, true);

    s.exportarDot("visualizacoes/splay/splay_teste_funcional.dot");
}


// ============================================================================
// BLOCO 4 -- TREAP
// ============================================================================
void testarTreap(){
    separador("TREAP");
    Treap t;

    checar("buscar em Treap vazia", t.buscar(5), false);
    t.remover(5);

    int valores[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    for (int v : valores) t.inserir(v);

    checar("buscar(45) presente",   t.buscar(45), true);
    checar("buscar(999) ausente",   t.buscar(999), false);

    t.remover(30);
    checar("apos remover(30): buscar(30)",        t.buscar(30), false);
    checar("apos remover(30): buscar(20) sobrevive", t.buscar(20), true);
    checar("apos remover(30): buscar(40) sobrevive", t.buscar(40), true);
    checar("apos remover(30): buscar(25) sobrevive", t.buscar(25), true);
    checar("apos remover(30): buscar(35) sobrevive", t.buscar(35), true);

    t.remover(50);
    checar("apos remover raiz(50): buscar(50)",   t.buscar(50), false);
    checar("apos remover raiz(50): buscar(70) sobrevive", t.buscar(70), true);
    checar("apos remover raiz(50): buscar(20) sobrevive", t.buscar(20), true);

    t.exportarDot("visualizacoes/treap/treap_teste_funcional.dot");
}


// ============================================================================
// BLOCO 5 -- KD-TREE
// ============================================================================
void testarKDTree(){
    separador("KD-TREE");
    KDTree k;

    checar("buscar em KDTree vazia", k.buscar(1.0, 1.0), false);

    double pontos[][2] = {{5,4}, {3,1}, {2,2}, {7,3}, {8,7}, {4,7}, {9,6}};
    for (auto& p : pontos) k.inserir(p[0], p[1]);

    checar("buscar ponto existente (4,7)",     k.buscar(4, 7),     true);
    checar("buscar ponto inexistente (0,0)",   k.buscar(0, 0),     false);
    checar("buscar ponto existente (2,2)",     k.buscar(2, 2),     true);

    k.inserir(-3.5, 2.25);
    checar("buscar ponto com coordenadas negativas/decimais", k.buscar(-3.5, 2.25), true);

    k.exportarDot("visualizacoes/kdtree/kdtree_teste_funcional.dot");

    // NOTA para o relatorio (Secao 6): esta KDTree nao implementa remover()
    // nem busca por vizinho mais proximo -- limitacoes conscientes, a
    // documentar como trabalho futuro / decisao de escopo.
}


// ============================================================================
int main(){
    testarTrie();
    testarPatricia();
    testarSplay();
    testarTreap();
    testarKDTree();

    cout << "\n============================================\n";
    cout << "RESUMO: " << (totalTestes - testesFalhos) << "/" << totalTestes
         << " testes passaram.\n";
    if (testesFalhos > 0){
        cout << "ATENCAO: " << testesFalhos << " teste(s) FALHARAM -- revise antes de prosseguir.\n";
    } else {
        cout << "Todas as estruturas passaram na validacao funcional.\n";
    }
    cout << "============================================\n";

    return testesFalhos > 0 ? 1 : 0;
}