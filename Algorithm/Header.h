#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <utility>
#include <string>
#include <map>
#include <sstream>
#include <time.h>


using std::vector;
using std::pair;
using std::make_pair;
using std::set;
using std::map;
using std::string;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::istringstream;
using std::endl;
using std::advance;

typedef vector<vector<int>> matrix;
typedef vector<int> vec;
typedef vector<pair<int, vec>> vec_pair;

void Arr_to_adj_list(const vec& tree, int cur_v, int parent, vec_pair& adj_list, vec& ind_to_color) { // created function with binary tree preorder_traversal
    int left = 2 * cur_v + 1;
    int right = 2 * cur_v + 2;
    if (cur_v > tree.size() || tree[cur_v] < 0) {
        return;
    }
    if (cur_v != 0) {
        adj_list.push_back(make_pair(cur_v, vec({ parent })));
    }
    adj_list[parent].second.push_back(cur_v);
    ind_to_color.push_back(tree[cur_v]);
    Arr_to_adj_list(tree, left, cur_v, adj_list, ind_to_color);
    Arr_to_adj_list(tree, right, cur_v, adj_list, ind_to_color);
}

void leaves_random_color(adj_list shape, int k_num) {
    srand(time(NULL));
    auto it = shape.begin();
    while (it != shape.end()) {
        int ind = it->first;
        auto arr = it->second; // hope that this is a pointer
        if (ind != 0 && arr.size() == 1) { // if vertex is not a root and it has only parent as neighboor => it is a leaf
            arr[0].second = rand() % k_num + 1;
        }
        ++it;
    }
}

vector<int> Generate_random_shape(int max_height, int k_num) {
    int shape_size = pow(2, max_height + 1) - 1;
    vector<int> tree_shape(shape_size, -1);
    tree_shape[0] = 0;
    srand(time(NULL));
    // int num_it = rand() % shape_size + 2;
    int num_it = pow(2, max_height);
    set<int> nodes;
    nodes.insert(0);

    for (int i = 0; i < num_it; ++i) {
        vector<int> nodes_vec(nodes.size(), 0);
        std::copy(nodes.begin(), nodes.end(), nodes_vec.begin());
        int index = nodes_vec[rand() % nodes.size()];
        if (index > shape_size / 2 - 2) {
            continue;
        }

        int l = 2 * index + 1;
        int r = 2 * index + 2;
        if (tree_shape[l] == 0 && tree_shape[r] == -1) {
            tree_shape[r] = 0;
            nodes.erase(index);
            nodes.insert(r);
        }
        else if (tree_shape[r] == 0 && tree_shape[l] == -1) {
            tree_shape[l] = 0;
            nodes.erase(index);
            nodes.insert(l);
        }
        else {
            int lr = rand() % 2 + 1;
            tree_shape[2 * index + lr] = 0;
            nodes.insert(2 * index + lr);
        }
    }
    // apply arr_to_adj_list here
    leaves_random_color(tree_shape, k_num);
    return tree_shape;
}

vector<int> Prune_shape(vector<int> shape) {
    for (int i = shape.size() - 1; i > -1; --i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (shape[i] == 0) {
            if (shape[right] == shape[left] && shape[left] > 0) {
                shape[i] = shape[left];
                shape[left] = -1;
                shape[right] = -1;
            }
            if (shape[left] > 0 && shape[right] == -1) {
                shape[i] = shape[left];
                shape[left] = -1;
            }
            if (shape[right] > 0 && shape[left] == -1) {
                shape[i] = shape[right];
                shape[right] = -1;
            }
        }
    }
    int new_height = 0;
    for (int i = shape.size() - 1; i > -1; --i) {
        if (shape[i] > 0) {
            new_height = std::log(i + 1) / std::log(2);
            break;
        }
    }
    vector<int> pruned_tree;
    for (int i = 0; i < pow(2, new_height + 1) - 1; ++i) {
        pruned_tree.push_back(shape[i]);
    }
    return pruned_tree;
}

vector<int> Shape_random_color(vector<int> shape) {
    srand(time(NULL));
    for (int i = shape.size() - 1; i > -1; --i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (shape[i] == 0) {
            if (shape[right] > 0 && shape[left] > 0) {
                shape[i] = (rand() % 2 == 0) ? shape[left] : shape[right];
            }
            if (shape[left] > 0 && shape[right] == -1) {
                shape[i] = shape[left];
            }
            if (shape[right] > 0 && shape[left] == -1) {
                shape[i] = shape[right];
            }
        }
    }
    return shape;
}

set<int> Get_vertices_to_recolor(const vector<int>& tree) {
    set <int> vertices_to_recolor;
    for (int i = tree.size() - 1; i > -1; --i) {
        unsigned int left = 2 * i + 1;
        unsigned int right = 2 * i + 2;
        if (right < tree.size() && tree[left] > 0 && tree[right] > 0 && tree[left] != tree[right]) {
            vertices_to_recolor.insert(i);
        }
    }
    return vertices_to_recolor;
}

int Get_random_vertex(const set<int>& vertices) {
    vector<int> v_vec(vertices.size(), 0);
    std::copy(vertices.begin(), vertices.end(), v_vec.begin());
    return v_vec[rand() % vertices.size()];
}

pair<vector<int>, int> Color_random_vertex(const vector<int>& tree) {
    vector<int> new_tree(tree);
    set<int> v_rec = Get_vertices_to_recolor(tree);
    int cur_v = Get_random_vertex(v_rec);
    int first_recolored = cur_v;
    new_tree[cur_v] = (new_tree[cur_v] == new_tree[cur_v * 2 + 1]) ? new_tree[cur_v * 2 + 2] : new_tree[cur_v * 2 + 1];
    while (cur_v != 0) {
        int cur_p = (cur_v - 1) / 2;

        int left = cur_p * 2 + 1;
        int right = cur_p * 2 + 2;
        if (new_tree[cur_p] != new_tree[left] && new_tree[cur_p] != new_tree[right]) {
            new_tree[cur_p] = (rand() % 2 == 0) ? new_tree[left] : new_tree[right];
        } else {
            break;
        }
        cur_v = cur_p;
    }
    return std::make_pair(new_tree, first_recolored);
}

vector<int> Merge_shapes(const vector<int>& left_shape, const vector<int>& right_shape) {
    vector<int> new_shape;
    for (unsigned int i = 0; i < left_shape.size(); ++i) {
        new_shape.push_back((left_shape[i] != 0) ? left_shape[i] : right_shape[i]);
    }
    return new_shape;
}

vector<vector<int>> Generate_all_transm_trees(const int root_id, vector<int>& shape) {
    vector<vector<int>> left_shapes, right_shapes, possible_shapes;

    unsigned int left_child_id = 2 * root_id + 1;
    unsigned int right_child_id = 2 * root_id + 2;

    if ((shape.size() <= left_child_id) || (shape[left_child_id] < 0 && shape[right_child_id] < 0)) {
        return vector<vector<int>>{ shape };
    }
    if (shape[left_child_id] < 0) { //when left child is empty
        possible_shapes = Generate_all_transm_trees(right_child_id, shape);
        for (unsigned int i = 0; i < possible_shapes.size(); ++i) {
            possible_shapes[i][root_id] = possible_shapes[i][right_child_id];
        }
        return possible_shapes;
    }
    if (shape[right_child_id] < 0) { //when right child is empty
        possible_shapes = Generate_all_transm_trees(left_child_id, shape);
        for (unsigned int i = 0; i < possible_shapes.size(); ++i) {
            possible_shapes[i][root_id] = possible_shapes[i][left_child_id];
        }
        return possible_shapes;
    }
    left_shapes = Generate_all_transm_trees(left_child_id, shape);
    right_shapes = Generate_all_transm_trees(right_child_id, shape);

    for (unsigned int i = 0; i < left_shapes.size(); ++i) {
        for (unsigned int j = 0; j < right_shapes.size(); ++j) {
            if (left_shapes[i][left_child_id] == right_shapes[j][right_child_id]) {
                left_shapes[i][root_id] = left_shapes[i][left_child_id];
                possible_shapes.push_back(Merge_shapes(left_shapes[i], right_shapes[j]));
            }
            else {
                left_shapes[i][root_id] = left_shapes[i][left_child_id];
                possible_shapes.push_back(Merge_shapes(left_shapes[i], right_shapes[j]));
                left_shapes[i][root_id] = right_shapes[j][right_child_id];
                possible_shapes.push_back(Merge_shapes(left_shapes[i], right_shapes[j]));
            }
        }
    }

    return possible_shapes;
}

vector<vector<int>> Build_transm_network(const vector<int>& transm_tree) {
    int max_val = *std::max_element(transm_tree.begin(), transm_tree.end() - 1);
    vector<vector<int>> transm_network(max_val + 1, vector<int>());
    set<pair<int, int>> pairs;
    for (unsigned int i = 0; i < (transm_tree.size() - 1) / 2; ++i) {
        int left_child_id = 2 * i + 1;
        int right_child_id = 2 * i + 2;
        if (transm_tree[i] != -1) {
            if (transm_tree[left_child_id] != -1 && (transm_tree[left_child_id] != transm_tree[i])) {
                pairs.insert(make_pair(transm_tree[i], transm_tree[left_child_id]));
            }
            if (transm_tree[right_child_id] != -1 && (transm_tree[right_child_id] != transm_tree[i])) {
                pairs.insert(make_pair(transm_tree[i], transm_tree[right_child_id]));
            }
        }
    }
    while (!pairs.empty()) {
        pair<int, int> p = *pairs.begin();
        transm_network[p.first].push_back(p.second);
        transm_network[p.second].push_back(p.first);
        pairs.erase(p);
        pairs.erase(make_pair(p.second, p.first));
    }
    return transm_network;
}

int Calc_s_metric(vector<vector<int>> transm_network) {
    vector<int> deg_arr;
    for (auto net : transm_network) {
        if (!net.empty()) {
            deg_arr.push_back(net.size());
        }
    }
    int s_metric = 0;
    for (unsigned int i = 0; i < deg_arr.size(); ++i) {
        for (unsigned int j = i + 1; j < deg_arr.size(); ++j) {
            s_metric += deg_arr[i] * deg_arr[j];
        }
    }
    return s_metric;
}

vector<int> MCMC_run(const vector<int>& tree, int n, vector<int>& s_metrics, vector<int>& recolored_vertices) {
    vector<int> old_tree(tree);
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        pair<vector<int>, int> new_tree_v = Color_random_vertex(old_tree);
        recolored_vertices.push_back(new_tree_v.second);
        vector<vector<int>> old_trans = Build_transm_network(old_tree);
        int old_s = Calc_s_metric(old_trans);
        s_metrics.push_back(old_s);
        vector<vector<int>> new_trans = Build_transm_network(new_tree_v.first);
        int new_s = Calc_s_metric(new_trans);
        if (rand() % (old_s + new_s) > old_s) {
            old_tree = new_tree_v.first;
        }
    }
    return old_tree;
}


void Save_trees(vector<vector<int>> shapes, string path) {
    ofstream fout(path);
    for (unsigned int i = 0; i < shapes.size(); ++i) {
        for (int el : shapes[i]) {
            fout << el << " ";
        }
        fout << endl;
    }
    fout.close();
}
void Save_tree(vector<int> shape, string path) {
    ofstream fout(path);
    for (int el : shape) {
        fout << el << " ";
    }
    fout.close();
}

vector<vector<int>> Read_trees(string path) {
    ifstream fin(path);
    string row;
    vector<vector<int>> shapes;
    while (getline(fin, row)) {
        int num;
        istringstream n_stream(row);
        vector<int> shape;
        while (n_stream >> num) {
            shape.push_back(num);
        }
        shapes.push_back(shape);
    }
    fin.close();
    return shapes;
}
vector<int> Read_tree(string path) {
    ifstream fin(path);
    string row;
    getline(fin, row);
    istringstream n_stream(row);
    int num;
    vector<int> shape;
    while (n_stream >> num) {
        shape.push_back(num);
    }
    fin.close();
    return shape;
}

void Save_transm_net(vector<vector<int>> transm_net, string path, int s = 0) {
    ofstream fout(path);
    if (s != 0) {
        fout << s << endl;
    }
    for (unsigned int i = 0; i < transm_net.size(); ++i) {
        if (!transm_net[i].empty()) {
            fout << i;
            for (int el : transm_net[i]) {
                fout << " " << el;
            }
            fout << endl;
        }
    }
    fout.close();
}