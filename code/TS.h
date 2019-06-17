#ifndef TS_H
#define TS_H

#include <map>
#include <ctime>
#include <cstdlib>
#include "graph.h"

#define TABUSIZE 20 //���ɱ�ĳ���
#define TABULENGTH 100 //���ɳ��ȣ�������������ѡȡ�ĵ�������
#define NEIGHBORSIZE 50 //ÿ�β��������ĸ���
#define ITERATIONTIME 1000//��������


struct Solution
{
	int isChoose[MAX]; //�ö����Ʊ�ʾ��Ӧ���Ƿ�ѡ��
	int value; //��Ӧ����С��������ֵ

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
	Graph G;	//������ͼ
	int nodeNum;
	Solution solution; //��ǰ��
	Solution neighbors[NEIGHBORSIZE]; //һ�ε����в����������
	Solution currentBest;
	map<Solution, int> tabuList; //���ɱ�ÿ�ε���������
	
	int determine[MAX];
	clock_t start, end;

	int randInt(int begin, int end);
	double randFloat();

	void init(); //���ɵ�һ����
	void generateNeighbors(); //Ѱ������
	Solution findBestNeighbor(); 
	void updateTabuList();
	void outputResult();
};



#endif
