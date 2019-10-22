#include <iostream>
#include <list>
#include <string.h>

using namespace std;
class Node{
public:
    int id;             
    list<string> edges; 
    list<string> edges_status;
    int color;
    bool ativo;
    Node(){
        ativo = true;
    }
    Node(int i){
        ativo = true;
        id = i;
    }
    void insert_edge(string nd){
        edges.emplace_back(nd);
        edges_status.emplace_back("true");
    }
    list<string> get_edges(){
        return edges;
    }
    void insert_color(int c){
        color = c;
    }
    int get_color(){
        return color;
    }
    int get_edges_size(){
        return edges.size();
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


void build(Node nodes[],list<string> lines){
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
            if(nd_edge!="")
                nd.insert_edge(nd_edge);

            nd.ativo=true;
            line.erase(0, pos + 1);
        }
        nodes[i] = nd;
        i++;
    }
    return;
}
bool simplify(int k, Node nodes[],int j){
    int n =j;
    int l =0;
    for(;l<j;l++){
        Node nd = nodes[l];
        cout<<"id: "<<nd.id<<endl<<"s : "<<nd.edges.size()<<endl;
        for(string str : nd.edges){
            cout<<"["<<str<<"] ";
        }
        cout<<endl<<endl;
    }

    while(n>0){
        int i  =0,menor = k+1,index=0;
        for(i = 0; i < j ; i++){
            Node nd = nodes[i];
            if(!nd.ativo)
                continue;
            int nd_edges = nd.edges.size();
            if(nd_edges< k && nd_edges<menor ){
                menor=nd_edges;
                index = i;
            }
        }
        cout<<"Push: "<<nodes[index].id<<endl;
        nodes[index].ativo=false;
        // for(i = 0 ;)
        n--;
    }

    return true;
}


int main()
{
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
    bool status[k-2];
    Node nodes[lines.size()];
    build(nodes,lines);

    i = k;
    // while (i >= 2){
        cout<<"K = "<<i<<endl<<endl;
        bool st=simplify(i,nodes,lines.size());
        // if(st==1){
        //     cout<<"k: "<<i<<"->"<<"true"<<endl;
        // }else{
        //     cout<<"k: "<<i<<"->"<<"false"<<endl;
        // }
        i--;
    // }

    return 0;
}
