//#include "stdc++.h" 
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include <functional>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>

using namespace std;

static const char* newline = "\n";


/**
Reading text file and assigning string variable
*/
string read_file(string filename) {
    string wall;
    ifstream File(filename); //Open file to read
    // Use a while loop together with the getline() function to read the file line by line
    while (getline(File, wall)) {
        cout << wall; //Print file contents to console
    }
    File.close(); //Closing file after it has been read
    return wall;
}
/*
vector<string>create_wallMapNew(const string& file)
{
    string wall = read_file("wall0.txt");
    char* delim = "\n";
    vector<string> wall_v;
    istringstream iss(wall);

    for (string token = wall.c_str(); std::getline(iss, token, (char)delim); )
    {
        wall_v.push_back(std::move(token));
    }

    return wall_v;
}
*/
/**
Parse the string read from the file to create a number wall to iterate through
*/
vector<vector<string>> create_wallMap(string file) {
    string wall = read_file(file);
    //char* str_wall = (char*)malloc(sizeof(char) * 50);
    //char* str_wall = (char*)malloc(strlen(wall.c_str())+100); // undefined behaviour now being caused here exceeding buffer size mayb?
    char* str_wall = (char*)malloc(strlen(wall.c_str()) + 100);
    strcpy(str_wall, wall.c_str());
    wall = strtok(str_wall, newline); //using newline as token
    vector<vector<string>> wall_v;
    while (wall.size() > 0) {
        vector<string> row;
        regex re("\\d{3}"); //match only 3 digit numbers
        smatch match;
        while (regex_search(wall, match, re)) {
            if (match.str(0).length() > 0) row.push_back(match.str(0));
        }
        if (row.size() > 0)
            wall_v.push_back(row);
        wall = strtok(NULL, newline);
    }
    //free(str_wall);
    return wall_v;
}

void divideNodes(vector<vector<int>> graph) {
    for (int i = 0; i < graph.size(); i++)
        for (int j = 0; j < graph.at(i).size(); j++)
            graph.at(i).at(j) = graph.at(i).at(j) / 990; //calculate nodes to get strength rating
}
/*
*Setting up graph for later operations 
*/
vector<vector<int>> create_graph(vector<vector<string>> wall, int startingNode) {
    vector<vector<int>> graph;
    int pP = startingNode + 1;
    int aP = startingNode;
    for (int i = 0; i < wall.size(); i++) {
        if (i % 2 == 0 && i != 0)
            pP += 1;
        else if (i != 0)
            aP -= 1;

        char* char_array = ""; //defrenced ptr fix later on 
        for (int j = aP; j < pP; j++) {
            strcpy(char_array, wall.at(i).at(j).c_str());
            graph.at(i).push_back(atoi(char_array));
        }
    }
    return graph;
}
/**
Comparing and multiplying lowest rated nodes
*/
void shortest_path(vector<vector<int>> graph, int r, int b) {
    if (r != 1) {
        vector<int> temp;
        temp.push_back(graph.at(r).at(b) * graph.at(r - 1).at(b - 1));
        temp.push_back(graph.at(r).at(b) * graph.at(r - 1).at(b));
        sort(temp.begin(), temp.end());
        graph.at(r).at(b) = temp.front();
    }
}

void left_node(vector<vector<int>> graph, int a) {
    graph.at(a).front() = graph.at(a).front() * graph.at(a).front();
}

void right_node(vector<vector<int>> graph, int b) {
    graph.at(b).front() = graph.at(b).back() * graph.at(b - 1).back();
}
/*
map<int, function<vector<vector<int>> >> side_node_calc = {
    {1,left_node},
    {2,right_node}
};
*/
/*
* Possibly attempt better use of Dijkstra's algorithm
*/
void current_path_calculation(vector<vector<int>> graph, int r) {

    left_node(graph, r);
    for (int i = 1; i < r; r++)
        shortest_path(graph, r, i);
    right_node(graph, r);
}

int main(int argc, char const* argv[])
{
    const char* file = "wall0.txt";
    int startingNode = 10;
    startingNode--;
    vector<vector<string>> wall = create_wallMap(file);
    //vector<string> wall = create_wallMapNew(file);
    vector<vector<int>> graph = create_graph(wall, startingNode);
    divideNodes(graph);
  
    for (int i = 0; i < graph.size(); i++)  //Loop to find shortest path
        current_path_calculation(graph, i);

    sort(graph.back().begin(), graph.back().end()); // back() on empty vector results in undefined behaviour
    printf("%.9f", graph.back().front());
    return 0;
}
