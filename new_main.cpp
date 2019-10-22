#include <iostream>
#include <list>
#include <string.h>
#include <map>

using namespace std;
class Node{
public:
    int id;
    map<int, bool> edges;
    int color;
    int qtd_edges;
    int bck_qtd_edges;
    bool ativo;
    Node(){
        ativo = true;
        qtd_edges = 0;
        bck_qtd_edges=0;
        id = -1;
    }
    Node(int i){
        ativo = true;
        qtd_edges = 0;
        bck_qtd_edges=0;
        id = i;
    }
    void insert_edge(int edge_id){
        edges[edge_id] = true;
        qtd_edges++;
        bck_qtd_edges++;
    }
    void clean_edge(int edge_id){
        map<int, bool>::iterator it;
        for (it = edges.begin(); it != edges.end(); it++){
            if (it->first == edge_id)
            {
                edges[edge_id] = false;
                qtd_edges--;
            }
        }
    }
    void restart_edges(){
        qtd_edges=bck_qtd_edges;
        map<int, bool>::iterator it;
        for (it = edges.begin(); it != edges.end(); it++){
            edges[it->first] = true;
        }
    }

    void remover(){
        ativo = false;
    }
};

list<string> read(){
    list<string> lines;
    string str;
    while (getline(cin, str)){
        lines.push_back(str + " \n");
    }
    return lines;
}

void build(Node nodes[], list<string> lines){
    int i = 0;
    for (string line : lines){
        size_t pos = 0;
        Node nd;
        // cria o node
        if ((pos = line.find(" -->")) != string::npos){
            int id = stoi(line.substr(0, pos));
            nd = Node(id);
            line.erase(0, pos + 4);
        }
        //preenche o node
        while ((pos = line.find(" ")) != string::npos || (pos = line.find(" \n")) != string::npos){
            string nd_edge = line.substr(0, pos);
            if (nd_edge != "")
            {
                int edge_id = stoi(nd_edge);
                nd.insert_edge(edge_id);
            }

            nd.ativo = true;
            line.erase(0, pos + 1);
        }
        nodes[i] = nd;
        i++;
    }
    return;
}

int lowest(int k, int j, Node nodes[]){
    int i = 0, index = 0;
    int lowest = 2147483647;
    bool has_value = false;
    int node_v = 0;
    for (i = 0; i < j; i++){
        Node nd = nodes[i];
        if (!nd.ativo)
            continue;
        int nd_edges = nd.qtd_edges;

        if (nd_edges < k){
            if(nd_edges<lowest){
                // cout<<"let it move you";
                lowest = nd_edges;
                node_v = nd.id;
                index = i;
                has_value = true;
            }
            // else if(nd_edges==lowest){
            //     if(nd.id<node_v){
            //         node_v=nd.id;
            //         index=i;
            //         has_value=true;
            //     }
            // }
        }
    }
    if (!has_value){
        // cout<<"Poropopo po po po po\nPoropopo po po po po\noropopo po po po po\nPoropopo po po po po";
        return -1;
    }

    return index;
}

int highest(int nd_size, Node nodes[]){
    int highest = -1, i = 0, index = 0;
    for (i = 0; i < nd_size; i++){
        Node nd = nodes[i];
        if (!nd.ativo)
            continue;
        int nd_edges = nd.qtd_edges;
        if (nd_edges > highest){
            highest = nd_edges;
            index = i;
        }
    }
    return index;
}

void remove_edge(Node nodes[], int index, int nd_size){
    int i = 0;
    for (i = 0; i < nd_size; i++){
        Node nd = nodes[i];
        if (!nd.ativo)
            continue;
        nd.clean_edge(index);
        nodes[i] = nd;
        // cout<<"id: "<<nd.id<<"->"<<nd.qtd_edges<<endl;
    }
}

bool simplify(int k, Node nodes[], int nodes_size){
    int n = nodes_size, i = 0;
    while (n > 0){
        int index = lowest(k, nodes_size, nodes);
        if (index < 0){
            index = highest(nodes_size, nodes);
            cout << "Push: " << nodes[index].id <<" *"<< endl;

        }else{
            cout << "Push: " << nodes[index].id << endl;
        }
        remove_edge(nodes, nodes[index].id, nodes_size);
        nodes[index].ativo = false;
        n--;
    }

    return true;
}

void restart(int nd_size,Node nodes[]){
    int i = 0 ;
    for( i = 0 ; i < nd_size ; i++ ){
        Node nd = nodes[i];
        nd.ativo = true;
        nd.restart_edges();
        nodes[i] = nd;
    }
}

void debug(int nd_size,Node nodes[]){
    int i = 0 ;
    for ( i = 0 ; i < nd_size ; i++ ){
        Node nd = nodes[i]; 
        if(nd.ativo){
            cout<<nd.id<<endl;
            map<int,bool>::iterator it;
            for( it = nd.edges.begin() ; it != nd.edges.end() ; it++){

                if(it->second){
                    cout<<"["<<it->first<<"] ";
                }
            }
            cout<<endl;
        }
        
    }
}

int main(){
    string line, graph;
    getline(cin, graph);
    getline(cin, line);
    graph.erase(0, 5);
    graph.erase(graph.end() - 1);
    line.erase(0, 2);
    cout << "Graph" + graph + " -> Physical Registers: " + line << endl;
    cout << "----------------------------------------" << endl;
    cout << "----------------------------------------" << endl;
    int k = stoi(line);
    int i = 0;
    list<string> lines = read();
    bool status[k - 2];
    Node nodes[lines.size()];
    build(nodes, lines);
    Node backup[lines.size()];

    i = k;
    while (i >= 2){
        debug(lines.size(),nodes);
        cout << "K = " << i << endl<< endl;
        bool st = simplify(i, nodes, lines.size());
        if(st==1){
            cout<<"k: "<<i<<"->"<<"true"<<endl;
        }else{
            cout<<"k: "<<i<<"->"<<"false"<<endl;
        }
        restart(lines.size(),nodes);
        i--;
        cout<<"----------------------------------------"<<endl;
    }

    return 0;
}
