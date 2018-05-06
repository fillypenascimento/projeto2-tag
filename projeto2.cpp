#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <fstream>
#include <queue>
#include <ctime>
#define GNUPLOT "gnuplot -persist"

using namespace std;
class Graph{
    int n_vertices;
    list<int> *lista_adj;

  public:
    Graph(int n_vertices);                                                            //construtor da classe grafo
    void CriaListaAdjacencias(string GraphFile);                                      //cria lista de adjacencias
    void ImprimeGrafo();                                                              //imprime grafo
    void KahnsAlgorithm();                                                            //ordenação topologica Kahn
    void DFS();                                                                       //inicialização de estruturas para ordenar o grafo por dfs
    void DFSCoreAlgorithm(int vertice, vector<int> &verifica_visitado, list<int> &L); //ordenação topologica dfs
    void GenFileToGraph(string File, double time_vertice, double time_total);         //gera o arquivo DFSData e KhanData para gerar o grafico
    void GenGraph();                                                                  //gera o grafico com os arquivos criados na funcao GenFileToGraph
};

Graph::Graph(int n_vertices){
    this->n_vertices = n_vertices;
    lista_adj = new list<int>[n_vertices];
}

void Graph::CriaListaAdjacencias(string GraphFile){
    string line, number;
    ifstream file(GraphFile);
    int i, j, source_number, target_number;

    getline(file, line);
    while (getline(file, line)){
        i = 0;
        while ((line[i] >= '0') && (line[i] <= '9')){
            i++;
        }
        number = line.substr(0, i);
        source_number = stoi(number);

        i++;
        j = i;

        while ((line[i] >= '0') && (line[i] <= '9')){
            i++;
        }
        number = line.substr(j, i);
        target_number = stoi(number);

        lista_adj[source_number].push_back(target_number);
    }
}

void Graph::ImprimeGrafo(){
    int i = 0, j;
    list<int>::iterator count;

    for (i; i != n_vertices; i++){
        cout << i << ": ";
        for (count = lista_adj[i].begin(); count != lista_adj[i].end(); ++count){
            j = *count;
            cout << j << " ";
        }
        cout << '\n';
    }
}

//com base no algortítmo apresentado em http://edirlei.3dgb.com.br/aulas/paa/PAA_Aula_07_Ordenacao_Topologica.pdf
void Graph::KahnsAlgorithm(){
    vector<int> I; //I é o vetor com os graus de saída de cada vértice
    stack<int> S;  //S é a pilha que recebe os vértices com grau de entrada 0
    list<int> L;   //L é a lista que representa o grafo ordenado topologicamente
    list<int>::iterator count;
    int x = 0, y, z;

    //loop para inicializar as posições no vetor I com a quantidade de vertices
    for (x = 0; x != n_vertices; x++){
        I.push_back(0);
    }

    //loop para checar todas as listas de adjacências e incrementar os graus dos vértices de entrada
    for (x = 0; x != n_vertices; x++){
        for (count = lista_adj[x].begin(); count != lista_adj[x].end(); ++count){
            y = *count;
            I[y] += 1;
        }
    }

    for (x = 0; x != n_vertices; x++){
        if (I[x] == 0){
            S.push(x);
        }
    }

    while (!S.empty()){
        z = S.top();
        S.pop();
        L.push_back(z);

        for (count = lista_adj[z].begin(); count != lista_adj[z].end(); ++count){
            y = *count;
            I[y] -= 1;
            if (I[y] == 0){
                S.push(y);
            }
        }
    }

    cout << "GRAFO ORDENADO POR KAHN:  ";
    for (count = L.begin(); count != L.end(); ++count){
        y = *count;
        cout << y << " ";
    }
}

//algoritmo DFS tomando certa base no encontrado em: http://professor.ufabc.edu.br/~leticia.bueno/classes/teoriagrafos/materiais/dfs.pdf
void Graph::DFS(){
    int i, x;
    list<int> L;
    vector<int> verifica_visitado;

    for (x = 0; x != n_vertices; x++){
        verifica_visitado.push_back(0);
    }

    for (x = 0; x != n_vertices; x++){
        if (verifica_visitado[x] == 0){
            DFSCoreAlgorithm(x, verifica_visitado, L);
        }
    }

    list<int>::iterator count;
    cout << "GRAFO ORDENADO POR DFS:   ";
    for (count = L.begin(); count != L.end(); ++count){
        i = *count;
        cout << i << " ";
    }
}

void Graph::DFSCoreAlgorithm(int vertice, vector<int> &verifica_visitado, list<int> &L){
    int v, check;
    list<int>::iterator count;

    verifica_visitado[vertice] = 1;

    for (count = lista_adj[vertice].begin(); count != lista_adj[vertice].end(); ++count){
        v = *count;
        if (verifica_visitado[v] == 0){
            DFSCoreAlgorithm(v, verifica_visitado, L);
        }
    }
    L.push_front(vertice);
}

void Graph::GenFileToGraph(string File, double time_vertice, double time_total){
    int vertice = 1;
    double time_graph = 0.0000;
    ofstream FileToGraph;

    //limpa o que já tiver escrito em data.txt para salvar novos dados e gerar o grafico
    FileToGraph.open(File, ofstream::out | ofstream::trunc);

    //loop que grava o tempo referente ao vertice usando ordenacao de Khan no arquivo KhanData10.txt
     while(time_graph <= time_total && vertice <= n_vertices){
        FileToGraph << time_graph << " " << vertice << endl;
        vertice++;
        time_graph = time_graph + time_vertice;
    }
    FileToGraph.close();
}

//feito com base na video aula em https://www.youtube.com/watch?v=SCDFotjMHTw&t= - Tutorial Gnuplot 4 - Plotando Gráficos a Partir de Arquivos de Texto
void Graph::GenGraph(){
    FILE *gnuplot;
    gnuplot = popen(GNUPLOT, "w");

    fprintf(gnuplot, "set key above center\n");
    fprintf(gnuplot, "set xlabel 'TEMPO'\n");
    fprintf(gnuplot, "set ylabel 'TOTAL DE VERTICES'\n");
    fprintf(gnuplot, "plot 'KhanData.txt' title 'KHAN ALGORITHM' lt rgb 'red' with lines smooth csplines, 'DFSData.txt' title 'DFS ALGORITHM' lt rgb 'blue' with lines smooth csplines\n");
    fclose(gnuplot);
}

int main(){
    double time_total, time_vertice;
    //time_total = tempo total gasto para rodar o algoritmo
    //time_vertice = tempo gasto em cada vertice 
    clock_t t;

    //Grafo de 10 vertices --------------------------------------------------------------------------------
    Graph grafo1(10);
    grafo1.CriaListaAdjacencias("top_small.txt");
    //grafo1.ImprimeGrafo();
    //cout << "\n\n";
    t = clock();
    grafo1.KahnsAlgorithm();
    t = clock() - t;
    time_total = ((float)t)/CLOCKS_PER_SEC;
    time_vertice = (((float)t)/CLOCKS_PER_SEC)/10; //calcula media de tempo em cada vertice
    cout << "\nPara ordenar e grafo de 10 vertices por Kahn levou " << time_total << " segundos.\n\n";
    grafo1.GenFileToGraph("KhanData.txt", time_vertice, time_total); // 
    
    t = clock();
    grafo1.DFS();
    t = clock() - t;
    time_total = ((float)t)/CLOCKS_PER_SEC;
    time_vertice = (((float)t)/CLOCKS_PER_SEC)/10; //calcula media de tempo em cada vertice
    cout << "\nPara ordenar e grafo de 10 vertices por DFS levou " << time_total << " segundos.\n\n";
    grafo1.GenFileToGraph("DFSData.txt", time_vertice, time_total);
    printf("Pressione ENTER para mostrar o Grafico");
    getchar();
    grafo1.GenGraph(); //Apos gerar os 2 arquivos texto com tempo de execução do DFS e do Khan, chama a funcao para plotar o grafico
    printf("Pressione ENTER para prosseguir com o proximo grafo\n");
    getchar();


    //Grafo de 100 vertices --------------------------------------------------------------------------------
    Graph grafo2(100);
    grafo2.CriaListaAdjacencias("top_med.txt");
    //grafo2.ImprimeGrafo();
    //cout << "\n\n";
    t = clock();
    grafo2.KahnsAlgorithm();
    t = clock() - t;
    time_total = ((float)t)/CLOCKS_PER_SEC;
    time_vertice = (((float)t)/CLOCKS_PER_SEC)/100; //calcula media de tempo em cada vertice
    cout << "\nPara ordenar e grafo de 100 vertices por Kahn levou " << time_total << " segundos.\n\n";
    grafo2.GenFileToGraph("KhanData.txt", time_vertice, time_total);
    
    t = clock();
    grafo2.DFS();
    t = clock() - t;
    time_total = ((float)t)/CLOCKS_PER_SEC;
    time_vertice = (((float)t)/CLOCKS_PER_SEC)/100; //calcula media de tempo em cada vertice
    cout << "\nPara ordenar e grafo de 100 vertices por DFS levou " << time_total << " segundos.\n\n";
    grafo2.GenFileToGraph("DFSData.txt", time_vertice, time_total);
    printf("Pressione ENTER para mostrar o Grafico");
    getchar();
    grafo2.GenGraph(); //Apos gerar os 2 arquivos texto com tempo de execução do DFS e do Khan, chama a funcao para plotar o grafico
    printf("Pressione ENTER para prosseguir com o proximo grafo");
    getchar();


    //Grafo de 10000 vertices --------------------------------------------------------------------------------
    Graph grafo3(10000);
    grafo3.CriaListaAdjacencias("top_large.txt");
    //grafo3.ImprimeGrafo();
    //cout << "\n\n";
    t = clock();
    grafo3.KahnsAlgorithm();
    t = clock() - t;
    time_total = ((float)t)/CLOCKS_PER_SEC;
    time_vertice = (((float)t)/CLOCKS_PER_SEC)/10000; //calcula media de tempo em cada vertice
    cout << "\nPara ordenar e grafo de 10000 vertices por Kahn levou " << time_total << " segundos.\n\n";
    grafo3.GenFileToGraph("KhanData.txt", time_vertice, time_total);


    t = clock();
    grafo3.DFS();
    t = clock() - t;
    time_total = ((float)t)/CLOCKS_PER_SEC;
    time_vertice = (((float)t)/CLOCKS_PER_SEC)/10000; //calcula media de tempo em cada vertice
    cout << "\nPara ordenar e grafo de 10000 vertices por DFS levou " << time_total << " segundos.\n\n";
    grafo3.GenFileToGraph("DFSData.txt", time_vertice, time_total);
    printf("Pressione ENTER para mostrar o Grafico");
    getchar();
    grafo3.GenGraph(); //Apos gerar os 2 arquivos texto com tempo de execução do DFS e do Khan, chama a funcao para plotar o grafico
    printf("Pressione ENTER para prosseguir com o proximo grafo");
    getchar();


    //Grafo de 100000 vertices --------------------------------------------------------------------------------
    Graph grafo4(100000);
    grafo4.CriaListaAdjacencias("top_huge.txt");
    //grafo4.ImprimeGrafo();
    //cout << "\n\n";
    t = clock();
    grafo4.KahnsAlgorithm();
    t = clock() - t;
    time_total = ((float)t)/CLOCKS_PER_SEC;
    time_vertice = (((float)t)/CLOCKS_PER_SEC)/100000; //calcula media de tempo em cada vertice
    cout << "\nPara ordenar e grafo de 100000 vertices por Kahn levou " << time_total << " segundos.\n\n";
    grafo4.GenFileToGraph("KhanData.txt", time_vertice, time_total);
    
    //Executa DFS no arquivo de 100000
    t = clock();
    grafo4.DFS();
    t = clock() - t;
    time_total = ((float)t)/CLOCKS_PER_SEC;
    time_vertice = (((float)t)/CLOCKS_PER_SEC)/100000; //calcula media de tempo em cada vertice
    cout << "\nPara ordenar e grafo de 100000 vertices por DFS levou " << time_total << " segundos.\n\n";
    grafo4.GenFileToGraph("DFSData.txt", time_vertice, time_total);
    printf("Pressione ENTER para mostrar o Grafico");
    getchar();
    grafo4.GenGraph(); //Apos gerar os 2 arquivos texto com tempo de execução do DFS e do Khan, chama a funcao para plotar o grafico
    printf("Pressione ENTER para terminar o programa");
    getchar();

    return 0;
}