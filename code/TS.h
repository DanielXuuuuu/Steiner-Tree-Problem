#ifndef TS_H
#define TS_H

#include <map>
#include <ctime>
#include <cstdlib>
#include "graph.h"

#define TABUSIZE 20 //禁忌表的长度
#define TABULENGTH 100 //禁忌长度，被禁对象不允许选取的迭代次数
#define NEIGHBORSIZE 50 //每次产生邻域解的个数
#define ITERATIONTIME 1000//迭代次数


struct Solution
{
	int isChoose[MAX]; //用二进制表示对应点是否被选择
	int value; //对应的最小生成树的值

	Solution operator=(const Solution& s) {
		this->value = s.value;
		for (int i = 0; i < MAX; i++) {
			this->isChoose[i] = s.isChoose[i];
		}
		return *this;
	}
	bool operator==(const Solution& s) {
		for (int i = 0; i < MAX; i++) {
			if (this->isChoose[i] != s.isChoose[i])
				return false;
		}
		return true;
	}
	bool operator <(const Solution& s) const {
		return this->value < s.value;
	}
};

class TS {
public:
	TS();
	void run();
private:
	Graph G;	//完整的图
	int nodeNum;
	Solution solution; //当前解
	Solution neighbors[NEIGHBORSIZE]; //一次迭代中产生的邻域解
	Solution currentBest;
	map<Solution, int> tabuList; //禁忌表，每次迭代都更新
	
	int determine[MAX];
	clock_t start, end;

	int randInt(int begin, int end);
	double randFloat();

	void init(); //生成第一个解
	void generateNeighbors(); //寻找邻域
	Solution findBestNeighbor(); 
	void updateTabuList();
	void outputResult();
};



#endif
