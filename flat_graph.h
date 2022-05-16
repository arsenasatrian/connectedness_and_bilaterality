#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>
#include<unordered_map>
#include<string.h>

using namespace std;

class Flat_Graph {
public:
    typedef vector<int> edge;
    
    Flat_Graph()
    	: node_count_(0)
        , bipartition_(true)
    	, bi_color_(0)
    	, cc_parent_(0)
        , node_list_(0)
    	, index2node(0)
        , edge_list_(0)
    { }
    
    ~Flat_Graph() {}

    void initialize();
    
    void check_first_line(stringstream& ss_line, bool& first_line);
    void check_line(const string& line_elem, int& line_iter);
    void read_from_file(const char* file_name);

    void add_node(const int v)
    {
        node_list_.push_back(v);
    }
    void add_edge(const int v1, const int v2)
    {
        vector<int> temp = { v1,v2 };
        edge_list_.push_back(temp);
    }
 
    int find_set(const int v) {
        if (v == cc_parent_[v])
	    return v;
        return cc_parent_[v] = find_set(cc_parent_[v]);
    }

    void check_connectedness(ofstream& f);
    void check_bilaterality(ofstream& f);

private:
    int node_count_;	
    bool bipartition_;
    vector<int> bi_color_;
    vector<int> cc_parent_;
    vector<int> node_list_;
    vector<string> index2node;
    vector<edge> edge_list_;
    unordered_map<string, int> node2index;
};

void Flat_Graph::initialize()
{
    bi_color_.resize(node_count_);
    cc_parent_.resize(node_count_);
    index2node.resize(node_count_);
    for (int i = 0; i < node_count_; ++i) {
        bi_color_[i] = -1;
        cc_parent_[i] = i;
    }
}

void Flat_Graph::check_connectedness(ofstream& f)
{
    int components_count = 0;
    for (int i = 0; i < node_count_; i++) {
    	components_count += (cc_parent_[i] == i);
    }
    for (int i = 0; i < node_count_; i++) {
    	cc_parent_[i] = find_set(cc_parent_[i]);
    }
    f << "The given graph has "<< components_count << " connected components\n";
    unordered_map<int, string> connected_components;
    string space = " ";
    for (int i = 0; i < node_count_; i++) {
	connected_components[cc_parent_[i]].append(index2node[i]);
	connected_components[cc_parent_[i]].append(space);
    }
    for (unordered_map<int, string>::iterator  it = connected_components.begin(); it != connected_components.end(); it++) {
        f << it->second << "\n";
    }
    if (components_count == 1) {
        f << "so graph is connected";
    }
    else {
        f << "so graph is not connected";
    }
}

void Flat_Graph::check_bilaterality(ofstream& f)
{
    if ( bipartition_ == true ) {
        f << "\nThe given graph is bipartite\n";
        for (int i = 0; i < bi_color_.size(); i++) {
            if (bi_color_[i] == 0) {
                f << index2node[i] << " ";
            }
        }
        f << "\n";
        for (int i = 0; i < bi_color_.size(); i++) {
            if (bi_color_[i] == 1) {
                f << index2node[i] << " ";
            }
        }
    } 
    else {
        f << "\nThe given graph is not bipartite";
    } 
}
size_t get_unique_num(size_t v1, size_t v2) 
{
    if (v1 > v2) swap(v1, v2);
    return (0.5 * (v1 + v2) * (v1 + v2 + 1)) + v2;
}

void Flat_Graph::check_first_line(stringstream& ss_line, bool& first_line)
{
    if (first_line) {
        string temp;
        if (strncmp(ss_line.str().c_str(), "vertex count =", 14)  == 0) {
            while (!ss_line.eof()) {
                ss_line >> temp;
                if (stringstream(temp)>> node_count_) {
                    initialize();
                    break;
                }
                else { node_count_ = -1; }
            }
            if (node_count_ == -1) {
                cout << "\nInvalid input of vertex count" << endl;
                exit (1);
            }
        }
        else {
            cout << "\nInvalid syntax input of vertex count" << endl;
            exit (1);
        }
        first_line = false;
    }
}
 
void Flat_Graph::check_line(const string& line_elem, int& l_iter)
{
    if (strcmp(line_elem.c_str(), "->") == 0) {
        ++l_iter;
    }
    else {
        cout << "Invalid syntax input of vertexes" << endl;
        exit (1);
    }
}

void Flat_Graph::read_from_file(const char* file_name) {
    ifstream file;
    file.open(file_name);
    if (file.is_open()) {
        string line, vertex;
        unordered_map<size_t, bool> unique_edges;
        size_t vertex_conut = 0;
        bool first_line = true;
        while (getline(file, line)) {
            stringstream ss_line;
            ss_line << line;
            check_first_line(ss_line, first_line);
            int line_iter = 0, head_vertex = 0;
            while (!ss_line.eof()) {
                ss_line >> vertex;
                if (line_iter == 1) {
                    check_line(vertex, line_iter);
                    continue;
                }
                if (node2index[vertex] == 0 && !(index2node[node2index[vertex]] == vertex)) {
                    node2index[vertex] = vertex_conut;
                    index2node[vertex_conut] = vertex;
                    add_node(vertex_conut);
                    ++vertex_conut;
                }
                if (line_iter == 0) head_vertex = node2index[vertex];
                if (line_iter > 0) {
                    if (unique_edges[get_unique_num(head_vertex, node2index[vertex])] == false) {
                        unique_edges[get_unique_num(head_vertex, node2index[vertex])] = true;
                        add_edge(head_vertex, node2index[vertex]);
                        cc_parent_[find_set(head_vertex)] = find_set(node2index[vertex]);
                    }
                }
                if (bi_color_[node2index[vertex]] == -1 && line_iter == 0) {
                    bi_color_[node2index[vertex]] = 1;
                }
                else if (bi_color_[node2index[vertex]] == -1 && line_iter > 0) {
                    bi_color_[node2index[vertex]] = 1 - bi_color_[head_vertex];
                }
                else if (bi_color_[node2index[vertex]] == bi_color_[head_vertex] && line_iter > 0) {
                    bipartition_ = false;
                }
                ++line_iter;
            }
        }
    }
    else {
        cout << "\nCannot open " << file_name << " file.";
        exit(1);
    }
    file.close();
}
