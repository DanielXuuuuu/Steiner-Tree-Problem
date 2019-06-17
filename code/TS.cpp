#include "TS.h"

TS::TS()
{
	for (int i = 0; i < MAX; i++) {
		determine[i] = -1;
		solution.isChoose[i] = 0;
	}
}

//返回[begin, end]范围内的一个随机整数
int TS::randInt(int begin, int end) {
	return rand() % (end - begin) + begin + 1;
}

//返回[0，1]的浮点数
double TS::randFloat() {
	return rand() / double(RAND_MAX);
}

void TS::run()
{
	srand((unsigned)time(NULL));
	G.readFile();
	start = clock();
	nodeNum = G.getNodeNum();

	int iteration = 0;
	init(); //生成初始解
	currentBest = solution;
	cout << "Iteration " << iteration << ": " << solution.value << "\tBest in all interations: " << currentBest.value << endl;
	while (iteration < ITERATIONTIME) {
		generateNeighbors();
		solution = findBestNeighbor();
		updateTabuList();
		iteration++;
		
		if (solution.value < currentBest.value) {
			currentBest = solution;
			end = clock();
		}
		cout << "iteration " << iteration << ": " << solution.value << "\tBest in all interations: " << currentBest.value << endl;
	}
	outputResult();
}

void TS::init() {
	//预先判断
	//1、如果度数为1，且非正则点，就一定不在Steiner最小树中
	//2、如果度数为1，且是正则点，与该点相邻的顶点一定在Steiner最小树中
	//3、如果点v度数为2，邻接点为x,y且xy相连，如果xv + yv > xy, 则点v一定不在Steiner最小树中
	int sum = 0;
	for (int i = 0; i < nodeNum; i++) {
		if (G.getDegree(i) == 1) {
			if (G.terminals[i] == 0) {
				determine[i] = 0;
			}
			else {
				for (int j = 0; j < nodeNum; j++) {
					if (G.matrix[i][j] != 0) {
						determine[j] = 1;
						break;
					}
				}
			}
		}
		if (G.getDegree(i) == 2 && G.terminals[i] == 0) {
			int node[2], k = 0;
			for (int j = 0; j < nodeNum; j++) {
				if (G.matrix[i][j] != 0) {
					node[k++] = j;
				}
			}
			if (G.matrix[node[0]][node[1]] != 0 && G.matrix[i][node[0]] + G.matrix[i][node[1]] > G.matrix[node[0]][node[1]]) {
				determine[i] = 0;
			}
		}

		if (G.terminals[i] == 1) {
			determine[i] = 1;
		}
	}

	do {
		for (int i = 0; i < nodeNum; i++) {
			if (determine[i] != -1) {
				solution.isChoose[i] = determine[i];
			}
			else {
				solution.isChoose[i] = (randFloat() > 0.5 ? 1 : 0);
			}
		}
	} while ((solution.value = (G.generateSubGraph(solution.isChoose, nodeNum)).evaluate()) == INF);
}

void TS::generateNeighbors()
{
	for (int i = 0; i < NEIGHBORSIZE; i++) {
		neighbors[i] = solution;
		int k = -1;
		do {
			if(k != -1)
				neighbors[i].isChoose[k] ^= 1;
			do {
				k = randInt(0, nodeNum - 1);
			} while (determine[k] != -1);
			neighbors[i].isChoose[k] ^= 1;
		} while ((neighbors[i].value = (G.generateSubGraph(neighbors[i].isChoose, nodeNum)).evaluate()) == INF);		
	}
}

Solution TS::findBestNeighbor()
{
	sort(neighbors, neighbors + NEIGHBORSIZE - 1, [](Solution a, Solution b)
	{return a.value < b.value; });

	int flag;
	for (int i = 0; i < NEIGHBORSIZE; i++) {
		flag = 1;
		for (auto it = tabuList.begin(); it != tabuList.end(); it++) {
			if (neighbors[i] == it->first) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			tabuList.insert(make_pair(neighbors[i],TABULENGTH));
			return neighbors[i];
		}
	}
	
	//特赦
	//cout << "特赦" << endl;
	for (auto it = tabuList.begin(); it != tabuList.end(); it++) {
		if (neighbors[0] == it->first) {
			it->second = TABULENGTH;
			break;
		}
	}
	return neighbors[0];
}

void TS::updateTabuList()
{ 
	for (auto it = tabuList.begin(); it != tabuList.end(); ) {
		it->second--;
		if (it->second == 0) {
			it = tabuList.erase(it);
		}
		else {
			it++;
		}
	}
}

void TS::outputResult()
{
	cout << "\nAfter " << ITERATIONTIME << " times iteration, the optimal minimum value is " << currentBest.value << endl;
	cout << "The steiner point is:";
	for (int i = 0; i < nodeNum; i++) {
		if (currentBest.isChoose[i] == 1 && G.terminals[i] == 0) {
			cout << " " << i + 1;
		}
	}
	cout << endl;
	cout << endl;
	cout << "Time spent from begin to the optimal value found: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
}
