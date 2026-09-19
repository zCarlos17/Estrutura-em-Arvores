#include "Trie.hpp"
#include "Patricia.hpp"
#include "Splay.hpp"
#include "Treap.hpp"
#include "KDTree.hpp"

#include <iostream>
#include <string>
using namespace std;

// ---------------------------------------------------------------------------
// Utilitarios de teste
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

    // ---- ESTADO 1: inicial, apos as insercoes ----
    t.exportarDot("visualizacoes/trie/trie_1_inicial.dot");

    checar("buscar(\"casa\") apos insercao",       t.buscar("casa"),       true);
    checar("buscar(\"casal\") apos insercao",      t.buscar("casal"),      true);
    checar("buscar(\"cas\") -- so prefixo, nao palavra", t.buscar("cas"),  false);
    checar("possuiPrefixo(\"cas\")",               t.possuiPrefixo("cas"), true);
    checar("possuiPrefixo(\"xy\") -- nao existe",  t.possuiPrefixo("xy"),  false);

    // ---- ESTADO 2: intermediario -- nova ramificacao ----
    
    t.inserir("casarioes");
    t.exportarDot("visualizacoes/trie/trie_2_ramificacao.dot");
    checar("apos inserir \"casarioes\": ela existe",        t.buscar("casarioes"), true);
    checar("apos inserir \"casarioes\": \"casarao\" intacta", t.buscar("casarao"),   true);

    // ---- ESTADO 3: pos-remocao ----
    
    t.remover("casa");
    t.exportarDot("visualizacoes/trie/trie_3_pos_remocao.dot");

    checar("apos remover \"casa\": buscar(\"casa\")",            t.buscar("casa"),  false);
    checar("apos remover \"casa\": buscar(\"casal\") sobrevive", t.buscar("casal"), true);
    checar("apos remover \"casa\": possuiPrefixo(\"casa\") ainda vale", t.possuiPrefixo("casa"), true);

    // casos de borda
    checar("buscar(\"\") em trie nao vazia",       t.buscar(""),           false);
    t.remover("naoexiste"); // nao deve travar nem afetar nada
    checar("apos remover chave inexistente: buscar(\"casal\") intacto", t.buscar("casal"), true);
}


// ============================================================================
// BLOCO 2 -- PATRICIA
// ============================================================================
void testarPatricia(){
    separador("PATRICIA");
    Patricia p;

    string palavras[] = {"casa", "casamento", "casamenteiro", "casarao", "casaria"};
    for (const string& s : palavras) p.inserir(s);

    // ---- ESTADO 1: inicial ----
    
    p.exportarDot("visualizacoes/patricia/patricia_1_inicial.dot");

    checar("buscar(\"casamenteiro\")",             p.buscar("casamenteiro"), true);
    checar("buscar(\"casame\") -- so prefixo",     p.buscar("casame"),       false);
    checar("possuiPrefixo(\"casame\") -- termina no meio de um rotulo comprimido",
           p.possuiPrefixo("casame"), true);

    // ---- ESTADO 2: intermediario -- DIVISAO de um rotulo comprimido ----
   
    p.inserir("casacos");
    p.exportarDot("visualizacoes/patricia/patricia_2_divisao.dot");
    checar("apos inserir \"casacos\": ela existe",          p.buscar("casacos"),   true);
    checar("apos inserir \"casacos\": \"casa\" intacta",     p.buscar("casa"),      true);
    checar("apos inserir \"casacos\": \"casarao\" intacta",  p.buscar("casarao"),   true);

    // ---- ESTADO 3: pos-remocao -- FUSAO de nos ----
    // ao remover "casarao", o no que sobra com um unico filho precisa ser
    // concatenado com ele, para manter a compactacao da Patricia.
    p.remover("casarao");
    p.exportarDot("visualizacoes/patricia/patricia_3_pos_fusao.dot");

    checar("apos remover \"casarao\": buscar(\"casarao\")",        p.buscar("casarao"), false);
    checar("apos remover \"casarao\": buscar(\"casaria\") sobrevive (no fundido)", p.buscar("casaria"), true);

    // remover palavra que e prefixo de outra (forca outra fusao)
    p.remover("casamento");
    checar("apos remover \"casamento\": buscar(\"casamento\")",       p.buscar("casamento"), false);
    checar("apos remover \"casamento\": buscar(\"casamenteiro\") sobrevive", p.buscar("casamenteiro"), true);

    // caso de borda: estrutura recem-criada
    Patricia vazia;
    checar("buscar em Patricia vazia",             vazia.buscar("qualquer"), false);
    checar("possuiPrefixo em Patricia vazia",      vazia.possuiPrefixo("a"), false);
}


// ============================================================================
// BLOCO 3 -- SPLAY
// ============================================================================
void testarSplay(){
    separador("SPLAY");
    Splay s;

    // caso de borda: operacoes em arvore vazia nao podem travar
    checar("buscar em Splay vazia",   s.buscar(10), false);
    s.remover(10);

    int valores[] = {50, 30, 70, 20, 40, 60, 80};
    for (int v : valores) s.inserir(v);

    // ---- ESTADO 1: inicial ----
    s.exportarDot("visualizacoes/splay/splay_1_inicial.dot");

    checar("buscar(40) presente",     s.buscar(40), true);
    checar("buscar(999) ausente",     s.buscar(999), false);

    // ---- ESTADO 2: intermediario -- o SPLAY em si ----
   
    s.buscar(40);
    s.exportarDot("visualizacoes/splay/splay_2_apos_splay_de_40.dot");
    checar("busca repetida apos splay nao corrompe a arvore", s.buscar(40), true);

    // ---- ESTADO 3: pos-remocao da raiz ----
    s.remover(40); // 40 esta na raiz apos as buscas acima
    s.exportarDot("visualizacoes/splay/splay_3_pos_remocao_raiz.dot");

    checar("apos remover raiz(40): buscar(40)",           s.buscar(40), false);
    checar("apos remover raiz(40): buscar(20) sobrevive", s.buscar(20), true);
    checar("apos remover raiz(40): buscar(80) sobrevive", s.buscar(80), true);

    // caso de borda: remocao total
    int restantes[] = {20, 30, 50, 60, 70, 80};
    for (int v : restantes) s.remover(v);
    checar("apos remover tudo: buscar(20)",       s.buscar(20),  false);
    checar("apos remover tudo: buscar(80)",       s.buscar(80),  false);

    s.remover(999); // remover de arvore ja vazia
    checar("remover de arvore ja vazia nao trava", true, true);
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

    // ---- ESTADO 1: inicial ----
   
    t.exportarDot("visualizacoes/treap/treap_1_inicial.dot");

    checar("buscar(45) presente",   t.buscar(45), true);
    checar("buscar(999) ausente",   t.buscar(999), false);

    // ---- ESTADO 2: intermediario -- insercao com rotacao por prioridade ----
    t.inserir(33);
    t.exportarDot("visualizacoes/treap/treap_2_pos_insercao.dot");
    checar("apos inserir 33: ela existe",        t.buscar(33), true);
    checar("apos inserir 33: 35 intacta",        t.buscar(35), true);
    checar("apos inserir 33: 30 intacta",        t.buscar(30), true);

    // ---- ESTADO 3: pos-remocao de no com dois filhos ----
    // remover 30 exige rotacoes ate ele virar folha
    t.remover(30);
    t.exportarDot("visualizacoes/treap/treap_3_pos_remocao.dot");

    checar("apos remover(30): buscar(30)",           t.buscar(30), false);
    checar("apos remover(30): buscar(20) sobrevive", t.buscar(20), true);
    checar("apos remover(30): buscar(40) sobrevive", t.buscar(40), true);
    checar("apos remover(30): buscar(25) sobrevive", t.buscar(25), true);
    checar("apos remover(30): buscar(35) sobrevive", t.buscar(35), true);

    // caso de borda: remover a raiz
    t.remover(50);
    checar("apos remover raiz(50): buscar(50)",           t.buscar(50), false);
    checar("apos remover raiz(50): buscar(70) sobrevive", t.buscar(70), true);
    checar("apos remover raiz(50): buscar(20) sobrevive", t.buscar(20), true);
}


// ============================================================================
// BLOCO 5 -- KD-TREE
// ============================================================================
void testarKDTree(){
    separador("KD-TREE");
    KDTree k;

    checar("buscar em KDTree vazia", k.buscar(1.0, 1.0), false);

    // conjunto classico de pontos 2D usado em livros-texto
    double pontos[][2] = {{5,4}, {3,1}, {2,2}, {7,3}, {8,7}, {4,7}, {9,6}};
    for (auto& p : pontos) k.inserir(p[0], p[1]);

    // ---- ESTADO 1: inicial -- particionamento espacial ----
    // cada nivel alterna a dimensao de corte (x, y, x, y ...)
    k.exportarDot("visualizacoes/kdtree/kdtree_1_inicial.dot");

    checar("buscar ponto existente (4,7)",     k.buscar(4, 7),     true);
    checar("buscar ponto inexistente (0,0)",   k.buscar(0, 0),     false);
    checar("buscar ponto existente (2,2)",     k.buscar(2, 2),     true);

    // ---- ESTADO 2: intermediario -- novo ponto no particionamento ----
    k.inserir(-3.5, 2.25);
    k.exportarDot("visualizacoes/kdtree/kdtree_2_pos_insercao.dot");
    checar("buscar ponto com coordenadas negativas/decimais", k.buscar(-3.5, 2.25), true);
    checar("apos nova insercao: (5,4) intacto",               k.buscar(5, 4),       true);

    // ---- ESTADO 3: pos-remocao da RAIZ ----
        k.remover(5, 4);
    k.exportarDot("visualizacoes/kdtree/kdtree_3_pos_remocao_raiz.dot");

    checar("apos remover raiz (5,4): ela sumiu",        k.buscar(5, 4), false);
    checar("apos remover raiz (5,4): (3,1) sobrevive",  k.buscar(3, 1), true);
    checar("apos remover raiz (5,4): (2,2) sobrevive",  k.buscar(2, 2), true);
    checar("apos remover raiz (5,4): (7,3) sobrevive",  k.buscar(7, 3), true);
    checar("apos remover raiz (5,4): (8,7) sobrevive",  k.buscar(8, 7), true);
    checar("apos remover raiz (5,4): (4,7) sobrevive",  k.buscar(4, 7), true);
    checar("apos remover raiz (5,4): (9,6) sobrevive",  k.buscar(9, 6), true);

    // remocao de no interno (nao folha, nao raiz)
    k.remover(3, 1);
    checar("apos remover (3,1): ela sumiu",             k.buscar(3, 1), false);
    checar("apos remover (3,1): (2,2) sobrevive",       k.buscar(2, 2), true);

    // casos de borda
    k.remover(999, 999); // ponto inexistente -- nao pode travar
    checar("remover ponto inexistente nao afeta a arvore", k.buscar(2, 2), true);

    // remocao total
    double restantes[][2] = {{2,2}, {7,3}, {8,7}, {4,7}, {9,6}, {-3.5, 2.25}};
    for (auto& p : restantes) k.remover(p[0], p[1]);
    checar("apos remover tudo: (8,7) sumiu",            k.buscar(8, 7), false);
    checar("apos remover tudo: (9,6) sumiu",            k.buscar(9, 6), false);

    KDTree vaziaKD;
    vaziaKD.remover(1, 1); // remover de arvore vazia -- nao pode travar
    checar("remover de KDTree vazia nao trava", true, true);

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