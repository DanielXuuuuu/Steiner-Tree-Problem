#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory.h>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

#define MAX 100
#define INF 0x3f3f3f3f

struct Edge {
	int start, end;
	int weight;
	Edge(int s, int e, int w) {
		start = s;
		end = e;
		weight = w;
	}
};

class Graph {
public:
	Graph();
	void addEdge(Edge e);
	void addNode();
	Graph generateSubGraph(int* chromosome, int n);

	void readFile();
	int evaluate(); //评价函数
	int getTerminalNum();
	int getNodeNum();
	int getEdgeNum();
	int getDegree(int node);
	int terminals[MAX];
	int matrix[MAX][MAX]; //邻接矩阵

private:
	int nodeNum;
	int edgeNum;
	int terminalNum;
	int degree[MAX];
	vector<Edge> edges;

	int father[MAX];//并查集的时候需要用到
	int findFather(int x);
	bool unionSet(int x, int y);
	int kruskal();
};

#endif // ! GRAPH_H

