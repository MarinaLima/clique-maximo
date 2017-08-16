#include <algorithm>
#include <iterator>
#include <iostream>
#include <chrono>
#include <time.h>
#include <vector>
#include <set>
#include <random>
#include <fstream>
#include <iomanip>
using namespace std;


struct vertice {
    int nome;
    int grau;
    //vector<int> adjacencias;
    set<int> adjacencias;
};

struct grafo {
    int nVertices;
    int nArestas;
    vector<vertice> vertices;
    vector<vector<char>> arestas;
};


vector<int> lerGrafo(char *nomeArquivo);
void printGrafo(grafo g);
bool testaClique(grafo sG);
bool comparaGrau(vertice a, vertice b);
bool menorGrau(vertice a, vertice b);
bool comparaNome(vertice a, vertice b);
void removeVertice(grafo *sG, int n);
grafo criaSubGrafo(vector<int> vertices);
bool isAdjacente(vertice a, int b);

grafo G;

int main(int argc, char *argv[]) {

    clock_t begin, end;
    double tempo;
    int wG = 0;
    grafo sG;
    vector<int> clique;
    vector<int> verticesSubgrafo;
    vector<int> ordem;


    if(argc == 1){
        cout << "Arquivo de entrada nao informado." << endl;
        return 0;
    }
    ordem = lerGrafo(argv[1]);

    //printGrafo(G);
    cout << "\n\nG = (" << G.nVertices << "," << G.nArestas << ")" << endl;

    begin = clock();

    for(int j=0; j<G.nVertices; j++) {

        verticesSubgrafo.clear();
        verticesSubgrafo.push_back(ordem[j]);
        for(auto it=G.vertices[ordem[j]].adjacencias.begin(); it!=G.vertices[ordem[j]].adjacencias.end(); ++it) {
            verticesSubgrafo.push_back(*it);
        }
        sort(verticesSubgrafo.begin(), verticesSubgrafo.end());

        sG = criaSubGrafo(verticesSubgrafo);


        while(!testaClique(sG) && sG.nVertices > wG) {

            sort(sG.vertices.begin(), sG.vertices.end(), comparaGrau);

            removeVertice(&sG, sG.vertices[0].nome);
            verticesSubgrafo.erase(verticesSubgrafo.begin());

        }

        if ((int)verticesSubgrafo.size() > wG) {
            clique.clear();
            clique = verticesSubgrafo;
            wG = sG.nVertices;

            // cout << endl << "w(G): " << wG << " | v={";
            // for (int k = 0; k < wG - 1; k++) {
            //     cout << clique[k] << ", ";
            // }
            // cout << clique[wG - 1] << "} | T = " << tempo << "s" << endl;
        }
    }


    end = clock();
    tempo = double(end - begin) / CLOCKS_PER_SEC;


    if(wG<3){
        cout << "Clique maxima nao encontrada." << endl;
    }
    else {
        cout << argv[1] << "\nw(G): " << wG << " | v={";
        for (int i=0; i<wG-1; i++) {
            cout << clique[i]+1 << ", ";
        }
        cout << clique[wG-1]+1 << "} | T = " << tempo << "s" << endl << endl;
    }

    return 0;
}

vector<int> lerGrafo(char *nomeArquivo) {
    int x, y;
    char t;
    vector<int> ordem;

    ifstream arquivo(nomeArquivo);

    if(!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de leitura." << endl;
        return ordem;
    }

    arquivo >> t >> t >> t >> t >> t;
    arquivo >> G.nVertices >> G.nArestas;

    vector<char> temp (G.nVertices, 0);

    //cria a matriz em branco e inicializa os vertices
    for(int i=0; i<G.nVertices; i++) {

        G.arestas.push_back(temp);

        vertice v = {
                i,              // nome
                0,              // grau
                set<int>()   // adjacencias
        };

        G.vertices.push_back(v);

    }

    //leitura das arestas
    for(int i=0; i<G.nArestas; i++) {

        arquivo >> t >> x >> y;

        G.arestas[x-1][y-1] = 1;
        G.arestas[y-1][x-1] = 1;

        G.vertices[x-1].grau++;
        G.vertices[x-1].adjacencias.insert(y-1);

        G.vertices[y-1].grau++;
        G.vertices[y-1].adjacencias.insert(x-1);

    }

    arquivo.close();

    sort(G.vertices.begin(), G.vertices.end(), comparaGrau);
    for(int i=0; i<G.nVertices; i++) {
        ordem.push_back(G.vertices[i].nome);
    }
    sort(G.vertices.begin(), G.vertices.end(), comparaNome);

    return ordem;

}


void printGrafo(grafo g) {

    cout << "G = (" << g.nVertices << "," << g.nArestas << ")" << endl << endl;

    for(int i=0; i<g.nVertices; i++) {

        cout << "Vertice " << g.vertices[i].nome << " (grau " << g.vertices[i].grau << "): ";

        for(auto itt=g.vertices[i].adjacencias.begin(); itt!=g.vertices[i].adjacencias.end(); ++itt){
            cout << *itt << " ";
        }

        cout << endl;

    }

    cout << endl << "MATRIZ DE ADJACENCIAS: " << endl;

    cout << "    ";
    for(int i=0; i<g.nVertices; i++) {
        cout << setw(2) << setfill(' ') << i << " ";
    }
    cout << endl << "  --";
    for(int i=0; i<g.nVertices; i++) {
        cout << "---";
    }
    cout << endl;
    for(int j=0; j<g.nVertices; j++) {
        cout << setw(2) << setfill(' ') << j << "| ";
        for(int i=0; i<g.nVertices; i++) {
            if (i==j) { cout << " - "; }
            else if (g.arestas[i][j]) { cout << " 1 "; }
            else { cout << " 0 "; }
            //cout << setw(3) << setfill(' ') << g.arestas[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl << endl;


}

bool testaClique(grafo sG) {

    for(int i=0; i<sG.nVertices-1; i++) {
        for(int j=i+1; j<sG.nVertices; j++) {
            //if(!G.arestas[sG.vertices[j].nome][sG.vertices[i].nome]) {
            if(!isAdjacente(sG.vertices[j], sG.vertices[i].nome)) {
                return false;
            }
        }
    }
    return true;
}

bool comparaGrau(vertice a, vertice b) {
    if (a.grau == b.grau) {
        return a.nome > b.nome;
    }
    return a.grau < b.grau;
}

bool comparaNome(vertice a, vertice b) {
    return a.nome < b.nome;
}

void removeVertice(grafo *sG, int n) {

    for(int i=0; i<sG->nVertices; i++) {

        if(sG->vertices[i].nome == n) {
            sG->vertices.erase(sG->vertices.begin()+i);
            sG->nVertices--;
        }

        else{
            if (sG->vertices[i].adjacencias.erase(n) == 1) {
                sG->vertices[i].grau--;
            }
        }

    }


}

grafo criaSubGrafo(vector<int> vertices) {

    grafo sG;
    sG.nVertices = G.nVertices;
    sG.nArestas = G.nArestas;
    sG.vertices = G.vertices;

    for(int i=0, j=0; i<sG.nVertices; i++) {

        if(vertices[j] == sG.vertices[i].nome) {
            j++;
        }

        else{
            removeVertice(&sG, sG.vertices[i].nome);
            i--;
        }

    }

    return sG;

}

bool isAdjacente(vertice a, int b) {
    auto it = a.adjacencias.find(b);
    if(it == a.adjacencias.end()) {
        return false;
    }
    return true;
}

