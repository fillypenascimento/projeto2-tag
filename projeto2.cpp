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
    void KahnsAlgorithm(int vertices);//ordenação topologica kahn
    //ordenação topologica dfs
    


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

void Graph::KahnsAlgorithm(int vertices){
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

    cout << "lista ordenada Kahn: ";
    for(count = L.begin(); count != L.end(); ++count){
        y = *count;
        cout << y << " ";
    }

}

int main() {

    string GraphFile = "top_small.txt";
    Graph grafo1(10);
    grafo1.CriaListaAdjacencias(GraphFile);
    grafo1.ImprimeGrafo(10);
    cout << "\n\n";
    grafo1.KahnsAlgorithm(10);

    return 0;
}