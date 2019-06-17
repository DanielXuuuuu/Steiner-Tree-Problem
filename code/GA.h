#ifndef GA_H
#define GA_H

#include <ctime>
#include <cstdlib>
#include "graph.h"

#define POPUSIZE 100	//种群个体数
#define TOURNAMENTSIZE 40
#define ELITISMNUMBER 10
#define CROSSEROVER_RATE 0.7
#define MUTATION_RATE 0.05
#define MAXGENS 1000

struct Individual {
	int chromosome[MAX]; //01串，来表示是否加入对应非terminal点，terminal点始终为1
	int fitness; //适应度,为简便，直接取对应的最小生成树大小（越小越好）

	Individual operator=(const Individual& indiv) {
		this->fitness = indiv.fitness;
		for (int i = 0; i < MAX; i++) {
			this->chromosome[i] = indiv.chromosome[i];
		}
		return *this;
	}
};

class GA {
public:
	GA();
	void run();

private:
	Graph G;	//完整的图
	int csLen;	//个体染色体数量
	Individual population[POPUSIZE], nextPopulation[POPUSIZE];
	
	int determine[MAX];

	int maxFitness;
	int repeat;

	clock_t start, end;

	int randInt(int begin, int end);
	double randFloat();
	Individual randomIndiv();	//随机生成可行解
	void initialize();	//初始化种群
	void evaluate();	//评价
	void elitism();		//筛选出精英个体（按fitness排序）
	Individual select();	//锦标赛选择
	void crossover(const Individual& p1, const Individual& p2, Individual& baby1, Individual& baby2);	//两点交叉
	void mutate();	//变异
	void outputResult();
};


#endif // !GA_H
