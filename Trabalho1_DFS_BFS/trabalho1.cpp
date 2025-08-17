#include "pilha.h"
#include "fila.h"
#include <limits>
#include <iostream>
using namespace std;

const int MAX_VERTICES = 10;

void mostrarGrafo(int G[][MAX_VERTICES], int numVertices){
  for(int i = 0; i < numVertices; i++){
      for(int j = 0; j < numVertices; j++){
          cout << G[i][j] << " ";
      }
      cout << "\n";
  }
}

bool visitado(int v, bool visitados[], int numVertices){
  if (v < 0 || v >= numVertices) {
    cout << "Vértice inválido!" << endl;
    return true; 
  }
  return visitados[v];
}

void visitar(int v, bool visitados[], int numVertices){
  if (v < 0 || v >= numVertices) {
    cout << "Vértice inválido!" << endl;
    return;
  }
  cout << "Visitando: " << v+1 << endl;
  visitados[v] = true;
}

void DFS(int v, int G[][MAX_VERTICES], int numVertices, bool visitados[]){
  Pilha S;
  criarPilha(S, MAX_VERTICES);
  empilhar(S, v);
  while(!vazia(S)){
    int atual;
    desempilhar(S, atual);
    
    if(!visitado(atual, visitados, numVertices)){
      visitar(atual, visitados, numVertices);
      
      for(int e = 0; e < numVertices; e++){
        if(G[atual][e] == 1 && !visitado(e, visitados, numVertices)){
          empilhar(S, e);
        }
      }
    } 
  } 
  destruirPilha(S);
}

void BFS(int v, int G[][MAX_VERTICES], int numVertices, bool visitados[]){
  Fila Q;
  criarFila(Q, MAX_VERTICES);
  enfileirar(Q, v);
  while(!vazia(Q)){
    int t;
    desenfileirar(Q, t);

    if(!visitado(t, visitados, numVertices)){
        visitar(t, visitados, numVertices);

        for(int i=0; i<numVertices; i++){
            if(G[t][i]==1 && !visitado(i, visitados, numVertices))
            enfileirar(Q, i);

        }
    }

  }
  
  destruirFila(Q);
}

// Pesquisa se existe caminho entre u e w usando BFS
bool existeCaminho(int u, int w, int G[][MAX_VERTICES], int numVertices){
    bool visitados[MAX_VERTICES] = {false};
    Fila Q;
    criarFila(Q, numVertices);
    enfileirar(Q, u);

    while(!vazia(Q)){
        int t;
        desenfileirar(Q, t);

        if(t == w){
            destruirFila(Q);
            return true;
        }

        if(!visitados[t]){
            visitados[t] = true;
            for(int i = 0; i < numVertices; i++){
                if(G[t][i] == 1 && !visitados[i]){
                    enfileirar(Q, i);
                }
            }
        }
    }

    destruirFila(Q);
    return false;
}

int main() {
    int grafo[MAX_VERTICES][MAX_VERTICES] = {{0}};
    int numVertices;

    // Vetor de visitados
    bool visitados[MAX_VERTICES];

   
// Número de vértices
do {
    cout << "Digite o número de vértices (max " << MAX_VERTICES << "): ";
    cin >> numVertices;

    if(cin.fail() || numVertices <= 0 || numVertices > MAX_VERTICES){
        cout << "Número inválido. Tente novamente." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else break;
} while(true);

// Inicializa matriz com 0
for(int i=0; i<numVertices; i++)
    for(int j=0; j<numVertices; j++)
        grafo[i][j] = 0;

// Pergunta se é dirigido
    bool dirigido;
    char resposta;
    cout << "O grafo é direcionado? (s/n): ";
    cin >> resposta;
    dirigido = (resposta == 's' || resposta == 'S');

    // Número de arestas
    int numArestas;
    cout << "Digite o número de arestas: ";
    cin >> numArestas;

    // Leitura das arestas
    cout << "Digite as arestas (u v):" << endl;
    for(int k = 0; k < numArestas; k++){
        int u, v;
        cin >> u >> v;
        u--; v--; // ajusta para 0..N-1
        if(u >= 0 && u < numVertices && v >= 0 && v < numVertices){
            grafo[u][v] = 1;
            if(!dirigido)
                grafo[v][u] = 1;
        } else {
            cout << "Aresta inválida! Ignorada.\n";
        }
    }

    int opcao;
    do {
        cout << "\n MENU \n";
        cout << "1. DFS\n";
        cout << "2. BFS\n";
        cout << "3. Pesquisar caminho entre dois vértices\n";
        cout << "4. Mostrar grafo\n";
        cout << "5. Sair\n";
        cout << "Escolha: ";
        cin >> opcao;

        switch(opcao){
            case 1: {
                int v;
                cout << "Vértice inicial (1 a " << numVertices << "): ";
                cin >> v;
                v--; // ajusta
                for(int i = 0; i < numVertices; i++) visitados[i] = false;
                cout << "\n DFS \n";
                DFS(v, grafo, numVertices, visitados);
                // percorre vértices não visitados (componentes desconexos)
                 for(int i = 0; i < numVertices; i++){
                    if(!visitados[i])
                     DFS(i, grafo, numVertices, visitados);
                    }

                break;
            }
            case 2: {
                int v;
                cout << "Vértice inicial (1 a " << numVertices << "): ";
                cin >> v;
                v--; // ajusta
                for(int i = 0; i < numVertices; i++) visitados[i] = false;
                cout << "\n BFS \n";
                BFS(v, grafo, numVertices, visitados);
                // percorre vértices não visitados (componentes desconexos)
                for(int i = 0; i < numVertices; i++){
                 if(!visitados[i])
                   BFS(i, grafo, numVertices, visitados);
                 }
                break;
            }
            case 3: {
                int u, w;
                cout << "Digite os vértices origem e destino: ";
                cin >> u >> w;
                u--; // ajusta
                w--;
                if(existeCaminho(u, w, grafo, numVertices))
                    cout << "Existe caminho de " << u+1 << " até " << w+1 << ".\n";
                else
                    cout << "Não existe caminho de " << u+1 << " até " << w+1 << ".\n";
                break;
            }
            case 4: {
                cout << "\n MATRIZ DO GRAFO \n";
                mostrarGrafo(grafo, numVertices);
                break;
            }
            case 5:
                cout << "Encerrando programa.\n";
                break;
            default:
                cout << "Opção inválida! Tente novamente.\n";
        }

    } while(opcao != 5);

    return 0;
}

//cd Trabalho1_DFS_BFS
//g++ trabalho1.cpp -o trabalho1.exe
//./trabalho1.exe