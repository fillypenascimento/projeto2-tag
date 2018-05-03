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
    Graph(int n_vertices);                                                                  //construtor da classe grafo
    void CriaListaAdjacencias(string GraphFile);                                            //cria lista de adjacencias
    void ImprimeGrafo();                                                                    //imprime grafo
    void KahnsAlgorithm();                                                                  //ordenação topologica Kahn
    void DFS();                                                                             //inicialização de estruturas para ordenar o grafo por dfs
    void DFSCoreAlgorithm(int vertice, vector<int> &verifica_visitado, list<int>   &L);     //ordenação topologica dfs
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

void Graph::ImprimeGrafo(){
    int i = 0, j;
    list<int>::iterator count;

    for(i; i != n_vertices; i++){
        cout << i << ": ";
        for(count = lista_adj[i].begin(); count != lista_adj[i].end(); ++count){
            j = *count;
            cout << j << " ";
        }
        cout << '\n';
    }
}

//com base no algortítmo apresentado em http://edirlei.3dgb.com.br/aulas/paa/PAA_Aula_07_Ordenacao_Topologica.pdf
void Graph::KahnsAlgorithm(){
    vector<int> I;  //I é o vetor com os graus de saída de cada vértice
    stack<int>  S;  //S é a pilha que recebe os vértices com grau de entrada 0
    list<int>   L;  //L é a lista que representa o grafo ordenado topologicamente

    int x = 0,y,z;

    list<int>::iterator count;

    //loop para inicializar as posições no vetor I com a quantidade de vertices
    for(x=0; x != n_vertices; x++){
        I.push_back(0);
    }

    //loop para checar todas as listas de adjacências e incrementar os graus dos vértices de entrada
    for(x=0; x != n_vertices; x++){
        
        for(count = lista_adj[x].begin(); count != lista_adj[x].end(); ++count){
            y = *count;
            I[y]+=1;
        }
    }

    for(x=0; x != n_vertices; x++){
        if(I[x] == 0){
            S.push(x);
        }
    }

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

    cout << "GRAFO ORDENADO POR KAHN:  ";
    for(count = L.begin(); count != L.end(); ++count){
        y = *count;
        cout << y << " ";
    }

}

//algoritmo DFS tomando certa base no encontrado em: http://professor.ufabc.edu.br/~leticia.bueno/classes/teoriagrafos/materiais/dfs.pdf
void Graph::DFS(){
    int i, x;
    list<int>   L;
    vector<int> verifica_visitado;

    for(x=0; x != n_vertices; x++){
        verifica_visitado.push_back(0);
    }

    for(x=0; x != n_vertices; x++){
        if(verifica_visitado[x] == 0){
            DFSCoreAlgorithm(x, verifica_visitado, L);
        }
    }

    list<int>::iterator count;
    cout << "GRAFO ORDENADO POR DFS:   ";
    for(count = L.begin(); count != L.end(); ++count){
        i = *count;
        cout << i << " ";
    }    
}

void Graph::DFSCoreAlgorithm(int vertice, vector<int> &verifica_visitado, list<int>   &L){
    int v, check, flag = 0;
    list<int>::iterator count;

    verifica_visitado[vertice] = 1;

    for(count = lista_adj[vertice].begin(); count != lista_adj[vertice].end(); ++count){
        v = *count;
        if(verifica_visitado[v] == 0){
            DFSCoreAlgorithm(v, verifica_visitado, L);
        }
    }
    
    if(L.empty()){
        L.push_front(vertice);
    }
    else{
        for(count = L.begin(); count != L.end(); ++count){
            v = *count;
            if(v == vertice){
                flag = 1;
            }
        }

        if(flag == 0){
            L.push_front(vertice);
        }   
    }
}

int main() {

    Graph grafo1(10);
    grafo1.CriaListaAdjacencias("top_small.txt");
    //grafo1.ImprimeGrafo();
    //cout << "\n\n";
    grafo1.KahnsAlgorithm();
    cout << "\n\n";
    grafo1.DFS();
    cout << "\n\n\n\n\n";

    

    Graph grafo2(100);
    grafo2.CriaListaAdjacencias("top_med.txt");
    //grafo2.ImprimeGrafo();
    //cout << "\n\n";
    grafo2.KahnsAlgorithm();
    cout << "\n\n";
    grafo2.DFS();
    cout << "\n\n\n\n\n";
    
    Graph grafo3(10000);
    grafo3.CriaListaAdjacencias("top_large.txt");
    //grafo3.ImprimeGrafo();
    //cout << "\n\n";
    grafo3.KahnsAlgorithm();
    cout << "\n\n";
    grafo3.DFS();
    cout << "\n\n\n\n\n";

    Graph grafo4(100000);
    grafo4.CriaListaAdjacencias("top_huge.txt");
    //grafo4.ImprimeGrafo();
    //cout << "\n\n";
    grafo4.KahnsAlgorithm();
    cout << "\n\n";
    grafo4.DFS();
    cout << "\n\n\n\n\n";

    return 0;
}