#include <algorithm>
#include <iterator>
#include <iostream>
#include <chrono>
#include <time.h>
#include <vector>
#include <random>
#include <fstream>
#include <iomanip>
using namespace std;


struct vertice {
    int nome;
    int grau;
    vector<int> adjacencias;
};

struct grafo {
    int nVertices;
    int nArestas;
    vector<vertice> vertices;
    vector<vector<char>> arestas;
};


vector<int> lerGrafo(char *arquivo);
void printGrafo(grafo g);
bool testaClique(grafo sG);
bool comparaGrau(vertice a, vertice b);
bool menorGrau(vertice a, vertice b);
bool comparaNome(vertice a, vertice b);
void removeVertice(grafo *sG, int n);
grafo criaSubGrafo(vector<int> vertices);

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
    cout << "\nG = (" << G.nVertices << "," << G.nArestas << ")" << endl;

    begin = clock();

    for(int j=0; j<G.nVertices; j++) {

        //cout << j << endl;

        verticesSubgrafo.clear();
        verticesSubgrafo = G.vertices[ordem[j]].adjacencias;
        verticesSubgrafo.push_back(ordem[j]);
        sort(verticesSubgrafo.begin(), verticesSubgrafo.end());

        sG = criaSubGrafo(verticesSubgrafo);


        while (!testaClique(sG) && sG.nVertices > wG) {

            //cout << "          " << sG.nVertices << endl;

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
        cout << endl << "w(G): " << wG << " | v={";
        for (int i = 0; i < wG - 1; i++) {
            cout << clique[i] << ", ";
        }
        cout << clique[wG - 1] << "} | T = " << tempo << "s" << endl;
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

    vector<char> temp (G.nVertices+1, 0);

    //cria a matriz em branco e inicializa os vertices
    for(int i=0; i<=G.nVertices; i++) {

        G.arestas.push_back(temp);

        vertice v = {
                i,              // nome
                0,              // grau
                vector<int>()   // adjacencias
        };

        G.vertices.push_back(v);

    }

    //leitura das arestas
    for(int i=0; i<G.nArestas; i++) {

        arquivo >> t >> x >> y;

        G.arestas[x][y] = 1;
        G.arestas[y][x] = 1;

        G.vertices[x].grau++;
        G.vertices[x].adjacencias.push_back(y);

        G.vertices[y].grau++;
        G.vertices[y].adjacencias.push_back(x);

    }

    arquivo.close();

    //organiza vertices
    for(int i=1; i<=G.nVertices; i++) {
        sort(G.vertices[i].adjacencias.begin(), G.vertices[i].adjacencias.end());
    }

    sort(G.vertices.begin() + 1, G.vertices.end(), comparaGrau);
    for(int i=1; i<=G.nVertices; i++) {
        ordem.push_back(G.vertices[i].nome);
    }
    sort(G.vertices.begin(), G.vertices.end(), comparaNome);

    return ordem;


}


void printGrafo(grafo g) {

    cout << "G = (" << g.nVertices << "," << g.nArestas << ")" << endl << endl;

    for(int i=1; i<=g.nVertices; i++) {

        cout << "Vertice " << g.vertices[i].nome << " (grau " << g.vertices[i].grau << "): ";

        for(int j=0; j<g.vertices[i].grau; j++){
            cout << g.vertices[i].adjacencias[j] << " ";
        }

        cout << endl;

    }

    cout << endl << "MATRIZ DE ADJACENCIAS: " << endl;

    cout << "    ";
    for(int i=1; i<=g.nVertices; i++) {
        cout << setw(2) << setfill(' ') << i << " ";
    }
    cout << endl << "  --";
    for(int i=0; i<g.nVertices; i++) {
        cout << "---";
    }
    cout << endl;
    for(int j=1; j<=g.nVertices; j++) {
        cout << setw(2) << setfill(' ') << j << "| ";
        for(int i=1; i<=g.nVertices; i++) {
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

    for(int i=1; i<sG.nVertices; i++) {
        for(int j=i+1; j<=sG.nVertices; j++) {
            if(!G.arestas[sG.vertices[j].nome][sG.vertices[i].nome]) {
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

        sort(sG->vertices[i].adjacencias.begin(), sG->vertices[i].adjacencias.end());

        if(sG->vertices[i].nome == n) {
            sG->vertices.erase(sG->vertices.begin()+i);
            sG->nVertices--;
        }

        else{
            for(int j=0; j<sG->vertices[i].grau && sG->vertices[i].adjacencias[j]<=n; j++) {
                if(sG->vertices[i].adjacencias[j] == n) {
                    sG->vertices[i].adjacencias.erase(sG->vertices[i].adjacencias.begin()+j);
                    sG->vertices[i].grau--;
                    sG->nArestas--;
                }
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

