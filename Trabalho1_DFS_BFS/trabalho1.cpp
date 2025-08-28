#include "pilha.h"
#include "fila.h"
#include <limits>
#include <iostream>
using namespace std;

const int MAX_VERTICES = 10;

// Códigos de escape ANSI para cores (para exibir a matriz)
constexpr const char* RED = "\033[31m";   // Vermelho
constexpr const char* GREEN = "\033[32m"; // Verde
constexpr const char* RESET = "\033[0m";  // Resetar cor


// Exibe a matriz de adjacência com cores.
// 0 (sem aresta) = vermelho
// 1 (com aresta) = verde
void mostrarGrafo(int G[][MAX_VERTICES], int numVertices){
    for(int i = 0; i < numVertices; i++){
        for(int j = 0; j < numVertices; j++){
            if (G[i][j] == 0) {
                // Vermelho para aresta inexistente
                cout << RED << G[i][j] << RESET << " ";
            } else {
                // Verde para aresta existente
                cout << GREEN << G[i][j] << RESET << " ";
            }
        }
        cout << "\n";
    }
}


// Retorna "true" se o vértice já foi visitado. Se o índice for inválido, emite um aviso e retorna "true" para evitar acessos fora do vetor.
bool foiVisitado(int v, bool visitados[], int numVertices){
    if (v < 0 || v >= numVertices) {
        cout << "Aviso: vértice inválido acessado!\n";
        return true; 
    }
    return visitados[v];
}


// Marca o vértice como visitado e imprime a ação.
void visitar(int v, bool visitados[], int numVertices){
    if (v < 0 || v >= numVertices) {
        cout << "Vértice inválido!\n";
        return;
    }
    cout << "Visitando: " << v+1 << endl;
    visitados[v] = true;
}

// Função: BFS (busca em largura)
// Estratégia FIFO (primeiro que entra é o primeiro que sai).
void BFS(int v, int G[][MAX_VERTICES], int numVertices, bool visitados[]){
    Fila Q;
    criarFila(Q, MAX_VERTICES);

    if (v >= 0 && v < numVertices && !visitados[v])
        enfileirar(Q, v);

    while (true) {
        // percorre a componente atual
        while (!vazia(Q)) {
            int t;
            desenfileirar(Q, t);

            if (!foiVisitado(t, visitados, numVertices)) {
                visitar(t, visitados, numVertices);

                // enfileira vizinhos
                for (int i = 0; i < numVertices; ++i) {
                    if (G[t][i] == 1 && !foiVisitado(i, visitados, numVertices))
                        enfileirar(Q, i);
                }
            }
        }

        // componente atual terminada -> procura próximo vértice não visitado
        int proximo = -1;
        for (int i = 0; i < numVertices; ++i) {
            if (!visitados[i]) { proximo = i; break; }
        }
        if (proximo == -1) break; // todos visitados, termina BFS
        enfileirar(Q, proximo); // inicia nova componente
    }

    destruirFila(Q);
}

// Função: DFS (busca em profundidade)
// Estratégia LIFO (último que entra é o primeiro que sai).
void DFS(int v, int G[][MAX_VERTICES], int numVertices, bool visitados[]){
    Pilha S;
    criarPilha(S, MAX_VERTICES);

    // Se o início for válido e ainda não visitado, empilha; se inválido,
    // iremos procurar o próximo vértice não visitado abaixo.
    if (v >= 0 && v < numVertices && !visitados[v])
        empilhar(S, v);

    while (true) {
        // percorre a componente atual
        while (!vazia(S)) {
            int atual;
            desempilhar(S, atual);

            if (!foiVisitado(atual, visitados, numVertices)) {
                visitar(atual, visitados, numVertices);

                // empilha vizinhos (ordem invertida para manter comportamento original)
                for (int e = numVertices - 1; e >= 0; --e) {
                    if (G[atual][e] == 1 && !foiVisitado(e, visitados, numVertices)) {
                        empilhar(S, e);
                    }
                }
            }
        }

        // componente atual terminada -> procura próximo vértice não visitado
        int proximo = -1;
        for (int i = 0; i < numVertices; ++i) {
            if (!visitados[i]) { proximo = i; break; }
        }

        if (proximo == -1) break; // todos visitados, termina DFS
        empilhar(S, proximo); // inicia nova componente
    }

    destruirPilha(S);
}



// Verifica se existe caminho entre dois vértices (u -> w) usando BFS.
bool existeCaminho(int u, int w, int G[][MAX_VERTICES], int numVertices){
    bool visitados[MAX_VERTICES] = {false}; // Marca quem já foi visitado
    Fila Q;
    criarFila(Q, numVertices);

    // Inicia a busca com o vértice u
    enfileirar(Q, u);

    // Enquanto houver vértices para explorar
    while(!vazia(Q)){
        int t;
        desenfileirar(Q, t);

        // Se chegamos ao destino, já podemos encerrar
        if(t == w){
            destruirFila(Q);
            return true;
        }

        // Se ainda não foi visitado, processamos
        if(!visitados[t]){
            visitados[t] = true;

            // Adiciona todos os vizinhos não visitados
            for(int i = 0; i < numVertices; i++){
                if(G[t][i] == 1 && !visitados[i]){
                    enfileirar(Q, i);
                }
            }
        }
    }

    destruirFila(Q);
    return false; // Não chegamos ao destino
}

// Retorna verdadeiro se o vértice está dentro dos limites válidos.
bool verticeExiste(int v, int numVertices) {
    return v >= 0 && v < numVertices;
}

void removerVertice(int G[][MAX_VERTICES], int &numVertices, int vertice) {
    if (numVertices <= 0) {
        cout << "Grafo vazio. Nada a remover.\n";
        return;
    }
    if (vertice < 1 || vertice > numVertices) {
        cout << "Índice de vértice inválido. Deve estar entre 1 e " << numVertices << ".\n";
        return;
    }
    int idx = vertice - 1; // 0-based

    // desloca linhas para cima a partir de idx
    for (int i = idx; i < numVertices - 1; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            G[i][j] = G[i+1][j];
        }
    }
    // desloca colunas para a esquerda a partir de idx
    for (int i = 0; i < numVertices - 1; ++i) {
        for (int j = idx; j < numVertices - 1; ++j) {
            G[i][j] = G[i][j+1];
        }
    }
    // zera última linha/coluna (agora fora do novo tamanho)
    int last = numVertices - 1;
    for (int i = 0; i < numVertices; ++i) {
        G[last][i] = 0;
        G[i][last] = 0;
    }

    numVertices--;
    cout << "Vértice " << vertice << " removido. Agora há " << numVertices << " vértices.\n";
}

void adicionarVertice(int G[][MAX_VERTICES], int &numVertices) {
    if (numVertices >= MAX_VERTICES) {
        cout << "Não é possível adicionar: limite de vértices (" << MAX_VERTICES << ") atingido.\n";
        return;
    }
    numVertices++;
    cout << "Vértice adicionado. Agora há " << numVertices << " vértices (último índice = " << numVertices << ").\n";
}

// Adiciona aresta/arco (1-based). Respeita se o grafo é direcionado.
void adicionarAresta(int G[][MAX_VERTICES], int numVertices, int u, int v, bool dirigido) {
    if (u < 1 || u > numVertices || v < 1 || v > numVertices) {
        cout << "Aresta inválida. Índices devem estar entre 1 e " << numVertices << ".\n";
        return;
    }
    u = u - 1;
    v = v - 1;
    G[u][v] = 1;
    if (!dirigido) G[v][u] = 1;
    cout << "Aresta adicionada \n";
}

// Remove aresta/arco (1-based). Respeita se o grafo é direcionado.
void removerAresta(int G[][MAX_VERTICES], int numVertices, int u, int v, bool dirigido) {
    if (u < 1 || u > numVertices || v < 1 || v > numVertices) {
        cout << "Aresta inválida. Índices devem estar entre 1 e " << numVertices << ".\n";
        return;
    }
    u = u - 1;
    v = v - 1;
    G[u][v] = 0;
    if (!dirigido) G[v][u] = 0;
   cout << "Aresta removida \n";
}

int main() {
    int grafo[MAX_VERTICES][MAX_VERTICES] = {{0}}; // Matriz de adjacência
    int numVertices;
    bool visitados[MAX_VERTICES]; // Vetor para marcar vértices visitados

    // Entrada: número de vértices 
    do {
        cout << "Digite o número de vértices (max " << MAX_VERTICES << "): ";
        cin >> numVertices;

        if(cin.fail() || numVertices <= 0 || numVertices > MAX_VERTICES){
            cout << "Número inválido. Tente novamente." << endl;
            cin.clear(); // Limpa estado de erro
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta lixo no buffer
        } else break;
    } while(true);

    // Inicializa a matriz com 0
    for(int i=0; i<numVertices; i++)
        for(int j=0; j<numVertices; j++)
            grafo[i][j] = 0;

    // Entrada: grafo direcionado ou não 
    bool dirigido;
    char resposta;
    do{
        cout << "O grafo é direcionado? (s/n): ";
        cin >> resposta;
        if(resposta == 's'){
            dirigido = true;
        } else if(resposta == 'n'){
            dirigido = false;
        } else {
            cout<<"Resposta inválida"<<endl;
        }
    }while(resposta != 's' && resposta != 'n');
    
    int u, v;
    //  Entrada: leitura das arestas
    cout << "Digite as arestas (u v e 0 0 caso queira parar):" << endl;
    do{
        cin >> u >> v;
        u--; v--; // Ajusta para índice base 0

        // Verifica validade antes de inserir
        if(verticeExiste(u, numVertices) && verticeExiste(v, numVertices)){
            grafo[u][v] = 1;
            if(!dirigido) // Se não for direcionado, adiciona na outra direção também
                grafo[v][u] = 1;
        }else if(u != -1 && v != -1) {
            cout << "Você terminou de digitar.\n";
        }else {
            cout << "Aresta inválida! Ignorada.\n";
        }
    }while(u != -1 || v != -1);

    // Menu de operações 
    int opcao;
    do {
        cout << "\n MENU \n";
        cout << "1. DFS\n";
        cout << "2. BFS\n";
        cout << "3. Pesquisar caminho entre dois vértices\n";
        cout << "4. Mostrar grafo\n";
        cout << "5. Pesquisar vértice\n";
        cout << "6. Sair\n";
        cout << "7. Adicionar vértice\n";
        cout << "8. Remover vértice\n";
        cout << "9. Adicionar aresta/arco\n";
        cout << "10. Remover aresta/arco\n";
        cout << "Escolha: ";
        cin >> opcao;

        switch(opcao){
            case 1: {
                // Executa DFS
                int v;
                cout << "Vértice inicial (1 a " << numVertices << "): ";
                cin >> v;
                v--; 
                fill(visitados, visitados+numVertices, false); // Zera visitados
                cout << "\n DFS \n";
                DFS(v, grafo, numVertices, visitados);

                break;
            }
            case 2: {
                // Executa BFS
                int v;
                cout << "Vértice inicial (1 a " << numVertices << "): ";
                cin >> v;
                v--; 
                fill(visitados, visitados+numVertices, false); // Zera visitados
                cout << "\n BFS \n";
                BFS(v, grafo, numVertices, visitados);

                break;
            }
            case 3: {
                // Pesquisa caminho entre dois vértices
                int u, w;
                cout << "Digite os vértices origem e destino: ";
                cin >> u >> w;
                u--; w--;
                if(existeCaminho(u, w, grafo, numVertices))
                    cout << "Existe caminho de " << u+1 << " até " << w+1 << ".\n";
                else
                    cout << "Não existe caminho de " << u+1 << " até " << w+1 << ".\n";
                break;
            }
            case 4: {
                // Exibe a matriz do grafo
                cout << "\n MATRIZ DO GRAFO \n";
                mostrarGrafo(grafo, numVertices);
                break;
            }
            case 5: {
                // Verifica se um vértice existe
                int v;
                cout << "Digite o vértice para pesquisar (1 a " << numVertices << "): ";
                cin >> v;
                v--; 
                if(verticeExiste(v, numVertices))
                    cout << "O vértice " << v+1 << " existe no grafo.\n";
                else
                    cout << "O vértice " << v+1 << " não existe no grafo.\n";
                break;
            }
            case 6:
                cout << "Encerrando programa.\n";
                break;
            case 7: { // Adicionar vértice
                adicionarVertice(grafo, numVertices);
                break;
            }
            case 8: { // Remover vértice
                int v;
                cout << "Digite o vértice a remover (1 a " << numVertices << "): ";
                if (!(cin >> v)) { cout << "Entrada inválida.\n"; cin.clear(); cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); break; }
                removerVertice(grafo, numVertices, v);
                break;
            }
            case 9: { // Adicionar aresta/arco
                int a,b;
                cout << "Digite a aresta (u v): ";
                if (!(cin >> a >> b)) { cout << "Entrada inválida.\n"; cin.clear(); cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); break; }
                adicionarAresta(grafo, numVertices, a, b, dirigido);
                break;
            }
            case 10: { // Remover aresta/arco
                int a,b;
                cout << "Digite a aresta a remover (u v): ";
                if (!(cin >> a >> b)) { cout << "Entrada inválida.\n"; cin.clear(); cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); break; }
                removerAresta(grafo, numVertices, a, b, dirigido);
                break;
            }
            default:
                cout << "Opção inválida! Tente novamente.\n";
        }

    } while(opcao != 6);

    return 0;
}


//cd Trabalho1_DFS_BFS
//g++ trabalho1.cpp -o trabalho1.exe
//./trabalho1.exe