#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <fstream>
#include <queue>
#include <ctime>

using namespace std;
class Graph{
    int n_vertices;
    list<int> *lista_adj;

    public:
    Graph(int n_vertices); //construtor da classe grafo
    void CriaListaAdjacencias(string GraphFile); //cria lista de adjacencias
    void ImprimeGrafo(int vertices); //imprime grafo
    void KahnsAlgorithm(int vertices);//ordenação topologica Kahn
    void DFSInit(int vertice);
    void DFSAlgorithm(int vertice, vector<int> verifica_visitado, list<int>   L); //ordenação topologica dfs
    


};

Graph::Graph(int n_vertices){
    this->n_vertices = n_vertices;
    lista_adj = new list<int>[n_vertices];
}

void Graph::CriaListaAdjacencias(string GraphFile){
    ifstream file(GraphFile);
    string line, number;
    int i, j, source_number, target_number;

    getline(file, line);
    while(getline(file, line)){
        i = 0;
        while((line[i] >= '0') && (line[i] <= '9')){
            i++;
        }
        number = line.substr(0,i);
        source_number = stoi(number);

        i++;
        j = i;

        while((line[i] >= '0') && (line[i] <= '9')){
            i++;
        }
        number = line.substr(j,i);
        target_number = stoi(number);

        lista_adj[source_number].push_back(target_number);
    }
}

void Graph::ImprimeGrafo(int vertices){
    int i = 0, j;
    list<int>::iterator count;

    for(i; i != vertices; i++){
        
        cout << i << ": ";
        for(count = lista_adj[i].begin(); count != lista_adj[i].end(); ++count){
            j = *count;
            cout << j << " ";
        }
        cout << '\n';
    }


}

void Graph::KahnsAlgorithm(int vertices){ //com base no algortítmo apresentado em http://edirlei.3dgb.com.br/aulas/paa/PAA_Aula_07_Ordenacao_Topologica.pdf
    vector<int> I;  //I é o vetor com os graus de saída de cada vértice
    stack<int>  S;  //S é a pilha que recebe os vértices com grau de entrada 0
    list<int>   L;  //L é a lista que representa o grafo ordenado topologicamente

    int x = 0,y,z;

    list<int>::iterator count;

    //loop para inicializar as posições no vetor I com a quantidade de vertices
    for(x=0; x != vertices; x++){
        I.push_back(0);
    }

    //loop para checar todas as listas de adjacências e incrementar os graus dos vértices de entrada
    for(x=0; x != vertices; x++){
        
        for(count = lista_adj[x].begin(); count != lista_adj[x].end(); ++count){
            y = *count;
            I[y]+=1;
        }
    }

    // cout << "graus: \n";
    // for(x=0; x != vertices; x++){
    //     cout << I[x] << "\n";
    // }

    for(x=0; x != vertices; x++){
        if(I[x] == 0){
            S.push(x);
        }
    }

    // for(stack<int> dump = S; !dump.empty(); dump.pop())
    //     cout << dump.top() << '\n';

    while(!S.empty()){
        z = S.top();
        S.pop();
        L.push_back(z);

        for(count = lista_adj[z].begin(); count != lista_adj[z].end(); ++count){
            y = *count;
            I[y]-=1;
            if(I[y] == 0){
                S.push(y);
            }
        }
    }

    cout << "LISTA ORDENADA KAHN:  ";
    for(count = L.begin(); count != L.end(); ++count){
        y = *count;
        cout << y << " ";
    }

}

void Graph::DFSInit(int vertice){
    //vector<int> I;  //I é o vetor com os graus de saída de cada vértice
    //stack<int>  S;  //S é a pilha que recebe os vértices com grau de entrada 0
    list<int>   L;  //L é a lista que representa o grafo ordenado topologicamente
    vector<int> verifica_visitado;
    list<int>::iterator count;
    int i, x;

    // for(x=0; x != n_vertices; x++){
    //     I.push_back(0);
    // }

    //loop para checar todas as listas de adjacências e incrementar os graus dos vértices de entrada
    // for(x=0; x != n_vertices; x++){
        
    //     for(count = lista_adj[x].begin(); count != lista_adj[x].end(); ++count){
    //         i = *count;
    //         I[i]+=1;
    //     }
    // }

    // for(x=0; x != n_vertices; x++){
    //     if(I[x] == 0){
    //         S.push(x);
    //     }
    // }


    for(i=0; i != n_vertices; i++){
        verifica_visitado.push_back(0);
    }


    // while(!S.empty()){
    //     DFSAlgorithm(S.top(), verifica_visitado, L);
    //     S.pop();
    // }
    DFSAlgorithm(vertice, verifica_visitado, L);

    cout << "\nLISTA ORDENADA DFS:  ";
    for(count = L.begin(); count != L.end(); ++count){
        i = *count;
        cout << i << " ";
    }
    
}

void Graph::DFSAlgorithm(int vertice, vector<int> verifica_visitado, list<int>   L){
    list<int>::iterator count;
    int v;
    
    if(verifica_visitado[vertice] == 0){
        verifica_visitado[vertice] = 1;

        for(count = lista_adj[vertice].begin(); count != lista_adj[vertice].end(); ++count){
            v = *count;
            DFSAlgorithm(v, verifica_visitado, L);
        }
        L.push_front(v);
    }
}

int main() {

    Graph grafo1(10);
    grafo1.CriaListaAdjacencias("top_small.txt");
    //grafo1.ImprimeGrafo(10);
    cout << "\n\n";
    grafo1.KahnsAlgorithm(10);
    grafo1.DFSInit(9);
    grafo1.DFSInit(6);

    Graph grafo2(100);
    grafo2.CriaListaAdjacencias("top_med.txt");
    //grafo2.ImprimeGrafo(100);
    //cout << "\n\n";
    //grafo2.KahnsAlgorithm(100);
    
    Graph grafo3(10000);
    grafo3.CriaListaAdjacencias("top_large.txt");
    //grafo3.ImprimeGrafo(10000);
    //cout << "\n\n";
    //grafo3.KahnsAlgorithm(10000);

    Graph grafo4(100000);
    grafo4.CriaListaAdjacencias("top_huge.txt");
    //grafo4.ImprimeGrafo(100000);
    //cout << "\n\n";
    //grafo4.KahnsAlgorithm(100000);

    return 0;
}