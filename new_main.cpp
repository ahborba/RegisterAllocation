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
    bool is_available(int color){
        map<int, bool>::iterator it;
        for (it = edges.begin(); it != edges.end(); it++){
            if(it->first==color){
                return false;
            }
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
    int node_v;
    for (i = 0; i < nd_size; i++){
        Node nd = nodes[i];
        if (!nd.ativo)
            continue;
        int nd_edges = nd.qtd_edges;
        if (nd_edges > highest){
            highest = nd_edges;
            index = i;
            node_v = nd.id;
        }else if(nd_edges == highest){
            if(nd.id < node_v){
                index = i;
                node_v = nd.id;
            }

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

list<Node> simplify(int k, Node nodes[], int nodes_size){
    list<Node> stack;
    int n = nodes_size, i = 0;
    while (n > 0){
        int index = lowest(k, nodes_size, nodes);
        if (index < 0){
            index = highest(nodes_size, nodes);
            cout << "Push: " << nodes[index].id <<" *"<< endl;

        }else{
            cout << "Push: " << nodes[index].id << endl;
        }
        stack.push_back(nodes[index]);
        remove_edge(nodes, nodes[index].id, nodes_size);
        nodes[index].ativo = false;
        n--;
    }

    return stack;
}

void restart(int nd_size,Node nodes[]){
    int i = 0 ;
    for( i = 0 ; i < nd_size ; i++ ){
        Node nd = nodes[i];
        nd.ativo = true;
        nd.color=-1;
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
Node get_node_by_id(int size , int id,Node nodes[]){
    int i = 0; 
    for (i = 0 ; i < size ; i++){
        Node nd = nodes[i];
        if(id==nd.id){
            return nd;
        }
    }
    return Node(-1);
}
void set_node_by_id(Node nd, int size, Node nodes[]){
    int i = 0; 
    for (i = 0 ; i < size ; i++){
        Node n = nodes[i];
        if(n.id==nd.id){
            nodes[i]=nd;
            return;
        }
    }
}



int verify_available_color(int k ,Node nd, Node nodes[],int size){
    int i = 0 ;

    while(i<k){
        // a cor não está presente diretamente em nenhuma aresta
        if(nd.is_available(i)){
            bool is_available = true;
            map<int,bool>::iterator it;
            //verifica se alguma aresta esta utilizando esta cor
            for( it = nd.edges.begin() ; it != nd.edges.end(); it++){
                int id = it->first;
                Node nd_edge = get_node_by_id(size,id,nodes);
                if(nd_edge.color == i){
                    is_available = false;
                }

            }
            if(is_available){
                return i;
            }
            
        }
        i++;
    }
    return -1;
}




bool select(int k ,list<Node> stack,Node nodes[]){
    int i = 0 , size = stack.size();

    for( i = 0 ; i < size ; i++){
        Node nd = stack.back();
        int color = verify_available_color(k,nd,nodes,size);
        if(color>=0){
            cout<<"Pop: "<<nd.id<<" -> "<<color<<endl;
            nd.color=color;
            set_node_by_id(nd,size,nodes);
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
    Node nodes[lines.size()];
    build(nodes, lines);
    Node backup[lines.size()];

    i = k;
    while (i >= 2){
        // debug(lines.size(),nodes);
        cout << "K = " << i << endl<< endl;
        list<Node> stack = simplify(i, nodes, lines.size());
        status[i] = select(i,stack,nodes);

        restart(lines.size(),nodes);
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
                cout<<"Graph"<<graph<<" -> K =  "<<i<<": Sucessful Allocation";
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