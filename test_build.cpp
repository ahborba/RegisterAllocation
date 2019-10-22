#include <iostream>
#include <list>
#include <string.h>
#include <map>


using namespace std;
class Node{
public:
    int id;
    list<Node> edges; 
    int color;
    int qtd_edges;
    bool ativo;
    Node(){
        id= -1;
        ativo=false;
        color = -1;
    }
    Node(int i){
        id = i;
        ativo = true;
        qtd_edges=0;
        color = -1;
    }
    void insert_edge(Node nd){
        for(Node n : edges){
            if(n.id==nd.id)
                return;
        }
        cout<<nd.id<<endl;
        edges.emplace_back(nd);
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


void build(map<int,Node> mp,list<string> lines){
    for (string line : lines){
        size_t pos = 0;
        Node nd_root;
        int id=0;
        if ((pos = line.find(" -->")) != string::npos){
            id = stoi(line.substr(0, pos));
            line.erase(0, pos + 4);
            nd_root = mp[id]; 
            if(nd_root.id==-1)
                nd_root = Node(id);
            
        }
        while ((pos = line.find(" ")) != string::npos || (pos = line.find(" \n")) != string::npos){
            string str_edge = line.substr(0, pos);
            if(str_edge!=""){
                int id_edge =stoi(str_edge);
                Node nd_edge = mp[id_edge];
                if(nd_edge.id == -1)
                    nd_edge = Node(id_edge);
                nd_edge.insert_edge(nd_root);
                nd_root.insert_edge(nd_edge);
                mp[id_edge]=nd_edge;
            }
            line.erase(0, pos + 1);
        }
        mp[id] = nd_root;

    }
    return;
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
    map<int,Node> nodes;
    build(nodes,lines);
    map<int,Node>::iterator it;
    for(it = nodes.begin(); it!=nodes.end(); ++it){
        cout<<'-';
    }

    

    return 0;
}
