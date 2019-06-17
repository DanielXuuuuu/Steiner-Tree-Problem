#ifndef GA_H
#define GA_H

#include <ctime>
#include <cstdlib>
#include "graph.h"

#define POPUSIZE 100	//��Ⱥ������
#define TOURNAMENTSIZE 40
#define ELITISMNUMBER 10
#define CROSSEROVER_RATE 0.7
#define MUTATION_RATE 0.05
#define MAXGENS 1000

struct Individual {
	int chromosome[MAX]; //01��������ʾ�Ƿ�����Ӧ��terminal�㣬terminal��ʼ��Ϊ1
	int fitness; //��Ӧ��,Ϊ��㣬ֱ��ȡ��Ӧ����С��������С��ԽСԽ�ã�

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
	Graph G;	//������ͼ
	int csLen;	//����Ⱦɫ������
	Individual population[POPUSIZE], nextPopulation[POPUSIZE];
	
	int determine[MAX];

	int maxFitness;
	int repeat;

	clock_t start, end;

	int randInt(int begin, int end);
	double randFloat();
	Individual randomIndiv();	//������ɿ��н�
	void initialize();	//��ʼ����Ⱥ
	void evaluate();	//����
	void elitism();		//ɸѡ����Ӣ���壨��fitness����
	Individual select();	//������ѡ��
	void crossover(const Individual& p1, const Individual& p2, Individual& baby1, Individual& baby2);	//���㽻��
	void mutate();	//����
	void outputResult();
};


#endif // !GA_H
