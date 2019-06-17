#include "graph.h"

Graph::Graph()
{
	nodeNum = 0;
	edgeNum = 0;
	terminalNum = 0;
	for (int i = 0; i < MAX; i++) {
		for(int j = 0; j < MAX; j++)
			matrix[i][j] = 0;
		terminals[i] = 0;
		degree[i] = 0;
	}
}

void Graph::addEdge(Edge e)
{
	edges.push_back(e);
	edgeNum++;
}

void Graph::addNode()
{
	nodeNum++;
}

Graph Graph::generateSubGraph(int * chromosome, int n) {
	Graph subGraph;
	int added[MAX];
	memset(added, 0, sizeof(added));
	for (int i = 0; i < n; i++) {
		if (chromosome[i] == 1) {
			for (int j = 0; j < n; j++) {
				if (matrix[i][j] != 0 && added[j] == 1)
					subGraph.addEdge(Edge(i, j, matrix[i][j]));
			}
			added[i] = 1;
			subGraph.addNode();
		}
	}
	return subGraph;
}

void Graph::readFile() {
	cout << "Please enter the name of instance file: ";
	string filename, line;
	cin >> filename;

	int start, end, weight, terminal;
	ifstream fin(filename, ios::in);
	if (!fin.is_open()) {
		cout << "Fail to open " << filename << endl;
		exit(1);
	}

	do {
		getline(fin, line);
	} while (line != "SECTION Graph");

	fin >> line >> nodeNum;
	fin >> line >> edgeNum;

	memset(matrix, 0, sizeof(matrix));

	for (int i = 0; i < edgeNum; i++) {
		fin >> line >> start >> end >> weight;
		matrix[start - 1][end - 1] = matrix[end - 1][start - 1] = weight;
		degree[start - 1]++;
		degree[end - 1]++;
		edges.push_back(Edge(start - 1, end - 1, weight));
	}

	do {
		getline(fin, line);
	} while (line != "SECTION Terminals" && line != "Section Terminals");

	fin >> line >> terminalNum;

	for (int i = 0; i < terminalNum; i++) {
		fin >> line >> terminal;
		terminals[terminal - 1] = 1;
	}

	fin.close();
	cout << "read file successfully! nodeNum = " << nodeNum << ", edgeNum = " << edgeNum << ", terminalNum = " << terminalNum << endl;
}

int Graph::evaluate()
{
	return kruskal();
}

int Graph::getTerminalNum()
{
	return terminalNum;
}

int Graph::getNodeNum()
{
	return nodeNum;
}

int Graph::getEdgeNum()
{
	return edgeNum;
}

int Graph::getDegree(int node)
{
	return degree[node];
}

int Graph::findFather(int x) {
	int root = x;
	while (root != father[root]) root = father[root];
	return root;
}

bool Graph::unionSet(int x, int y) {
	int fx = findFather(x);
	int fy = findFather(y);
	if (fx != fy) {
		father[fx] = fy;
		return true;
	}
	return false;
}

bool cmp(Edge a, Edge b) {
	return a.weight < b.weight;
}

int Graph::kruskal()
{
	for (int i = 0; i <= MAX; i++) {
		father[i] = i;
	}
	//n为边数，m为顶点数
	int nEdge = 0, res = 0;
	sort(edges.begin(), edges.end(), cmp);

	int i = 0;
	while (nEdge < nodeNum - 1 && i < edgeNum) {
		if (unionSet(edges[i].start, edges[i].end)) {
			res += edges[i].weight;
			nEdge++;
		}
		i++;
	}

	if (nEdge < nodeNum - 1)
		res = INF;
	return res;
}
