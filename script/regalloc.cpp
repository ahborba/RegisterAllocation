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
        color = -1;
        id = -1;
    }
    Node(int i){
        ativo = true;
        qtd_edges = 0;
        bck_qtd_edges=0;
        color = -1;
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
    bool is_available(int color){
        map<int, bool>::iterator it = edges.find(color);
        if(it != edges.end()){
            return false;
        }
        return true;
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


map<int,Node> nodes;

void debug(){
    int i = 0 ;
    cout<<"+-------------+\n";
    cout<<"|    DEBUG    |\n";
    cout<<"+-------------+\n";
    map<int,Node>::iterator it;
    for( it = nodes.begin() ; it != nodes.end() ; it++){
        Node nd = it->second;
        if(nd.ativo){
            cout<<"+-------------+\n";
            cout<<"|"<<"Id: "<<nd.id<<"       |"<<endl;
            cout<<"|"<<"Size: "<<nd.qtd_edges<<"      |"<<endl;
            cout<<"+-------------+\n";
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




list<string> read(){
    list<string> lines;
    string str;
    while (getline(cin, str)){
        lines.push_back(str + " \n");
    }
    return lines;
}

map<int,Node> build(list<string> lines){
    for (string line : lines){
        int id =0;
        size_t pos = 0;
        Node nd;
        // cria o node
        if ((pos = line.find(" -->")) != string::npos){
            id = stoi(line.substr(0, pos));
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
        nodes[id] = nd;
    }
    return nodes;
}

int lowest(int k){
    int i = 0, index = 0;
    int lowest = 2147483647;
    bool has_value = false;
    int node_v = 0;
    map<int,Node>::iterator it;
    for(it = nodes.begin();it!=nodes.end();it++){
        Node nd = it->second;
        if (!nd.ativo)
            continue;
        int nd_edges = nd.qtd_edges;
        if (nd_edges < k){
            if(nd_edges<lowest){
                lowest = nd_edges;
                node_v = nd.id;
                index = it->first;
                has_value = true;
            }
            else if(nd_edges==lowest){
                if(nd.id<node_v){
                    node_v=nd.id;
                    index=it->first;
                    has_value=true;
                }
            }
        }
    }
    if (!has_value){
        return -1;
    }

    return index;
}

int highest(){
    int highest = -1, i = 0, index = 0;
    int node_v;
    map<int,Node>::iterator it;
    for(it = nodes.begin();it!=nodes.end();it++){
        Node nd = it->second;
        if (!nd.ativo){
            // cout<<"Não ativo: "<<it->first<<endl;
            continue;
        }
        int nd_edges = nd.qtd_edges;
        if (nd_edges > highest){
            highest = nd_edges;
            index = it->first;
            node_v = nd.id;
        }else if(nd_edges == highest){
            if(nd.id < node_v){
                index = it->first;
                node_v = nd.id;
            }
        }
    }
    return index;
}

void remove_edge(int index){
    map<int,Node>::iterator it;
    for(it = nodes.begin();it!=nodes.end();it++){
        Node nd = it->second;
        if (!nd.ativo)
            continue;
        nd.clean_edge(index);
        it->second = nd;
    }
}

list<Node> simplify(int k,int nodes_size){
    list<Node> stack;
    int n = nodes_size, i = 0;
    while (n > 0){
        // debug();
        int index = lowest(k);
        if (index < 0){
            index = highest();
            cout << "Push: " << nodes[index].id <<" *"<< endl;

        }else{
            cout << "Push: " << nodes[index].id << endl;
        }
        stack.push_back(nodes[index]);
        remove_edge(nodes[index].id);
        nodes[index].ativo = false;
        Node nd = nodes[index];
        // cout<<"ATIVO:"<<nd.ativo<<endl;
        n--;
    }

    return stack;
}

void restart(){
    map<int,Node>::iterator it;
    for(it = nodes.begin();it!=nodes.end();it++){
        Node nd = it->second;
        nd.ativo = true;
        nd.color=-1;
        nd.restart_edges();
        nodes[it->first] = nd;
    }
}

int verify_available_color(int k ,Node nd){
    int i = 0 ;

    while(i<k){
        // a cor não está presente diretamente em nenhuma aresta
        // cout<<"color: "<<i<<endl;
        if(nd.is_available(i)){
            bool is_available = true;
            map<int,bool>::iterator it;
            for( it = nd.edges.begin() ; it != nd.edges.end() ; it++){
                int id = it->first;
                // cout<<"\t["<<id<<"]";
                map<int,Node>::iterator it_nd = nodes.find(id);
                if(it_nd == nodes.end()){
                    // cout<<"not a node"<<endl;
                    continue;
                }
                Node nd_edge = it_nd->second;
                // cout<<" color: "<<nd_edge.color<<endl;
                if(nd_edge.color==i){
                    is_available = false;
                    break;
                }
                is_available =true;
            }
            if(is_available)
                return i;
            
        }
        i++;
    }
    return -1;
}




bool select(int k ,list<Node> stack,map<int,Node> &nodes){
    int i = 0 , size = stack.size();

    for( i = 0 ; i < size ; i++){
        Node nd = stack.back();
        int color = verify_available_color(k,nd);
        if(color>=0){
            cout<<"Pop: "<<nd.id<<" -> "<<color<<endl;
            nd.color=color;
            nodes[nd.id] = nd;
            // cout<<"alterado: "<<nodes[nd.id].color;
            // set_node_by_id(nd,size,nodes);
        }else{
            cout<<"Pop: "<<nd.id<<" -> NO COLOR AVAILABLE"<<endl;
            return false;
        }


        stack.pop_back();
    }
    return true;
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
    // Node nodes[lines.size()];
    nodes = build(lines);
    map<int,Node>::iterator it;
    it = nodes.find(38);
    // if (it != nodes.end())
    //     cout<<"achou: "<<it->second.id<<endl;
    // else
    //     cout<<"PEW PEW PEW PEWEWEWEWEW"<<endl;


    i = k;
    // debug();
    while (i >= 2){
        cout << "K = " << i << endl<< endl;
        list<Node> stack = simplify(i,  lines.size());
        status[i] = select(i,stack,nodes);

        restart();
        i--;
        cout<<"----------------------------------------"<<endl;
    }
    cout<<"----------------------------------------"<<endl;
    for( i = k ; i >=2 ; i--){
        bool space = false;
        if(i<10)
            space = true;
        
        if(i>2){
            if(status[i]){
                if(space){
                    cout<<"Graph"<<graph<<" -> K =  "<<i<<": Successful Allocation"<<endl;
                }else{
                    cout<<"Graph"<<graph<<" -> K = "<<i<<": Successful Allocation"<<endl;
                }
            }
            else{
                if(space){
                    cout<<"Graph"<<graph<<" -> K =  "<<i<<": SPILL"<<endl;
                }else{
                    cout<<"Graph"<<graph<<" -> K = "<<i<<": SPILL"<<endl;
                }
            }
        }else{
            if(status[i])
                cout<<"Graph"<<graph<<" -> K =  "<<i<<": Successful Allocation";
            else
                cout<<"Graph"<<graph<<" -> K =  "<<i<<": SPILL";
        }
        

    }

    return 0;
}


/*
Graph 1 -> K = 21: Successful Allocation
Graph 1 -> K = 20: Successful Allocation
Graph 1 -> K = 19: Successful Allocation
Graph 1 -> K = 18: Successful Allocation
Graph 1 -> K = 17: Successful Allocation
Graph 1 -> K = 16: Successful Allocation
Graph 1 -> K = 15: Successful Allocation
Graph 1 -> K = 14: Successful Allocation
Graph 1 -> K = 13: Successful Allocation
Graph 1 -> K = 12: Successful Allocation
Graph 1 -> K = 11: Successful Allocation
Graph 1 -> K = 10: Successful Allocation
Graph 1 -> K =  9: Successful Allocation
Graph 1 -> K =  8: Successful Allocation
Graph 1 -> K =  7: Successful Allocation
Graph 1 -> K =  6: Successful Allocation
Graph 1 -> K =  5: SPILL
Graph 1 -> K =  4: SPILL
Graph 1 -> K =  3: SPILL
Graph 1 -> K =  2: SPILL
*/