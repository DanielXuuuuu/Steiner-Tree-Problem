#include "GA.h"

GA::GA()
{
	for (int i = 0; i < MAX; i++) {
		determine[i] = -1;
	}
	maxFitness = 0;
	repeat = 0;
}

void GA::run()
{
	srand((unsigned)time(NULL));
	G.readFile();
	start = clock();
	csLen = G.getNodeNum();
	int generation = 0;
	initialize();
	evaluate();
	elitism();
	cout << "Best individual in initial generation: " << population[0].fitness << endl;
	maxFitness = population[0].fitness;
	end = clock();
	repeat = 1;
	while (generation < MAXGENS && repeat <= 200) {
		int i;
		//交叉时保留精英个体
		for (i = ELITISMNUMBER; i + 1 < POPUSIZE; i += 2) {
			Individual p1 = select();
			Individual p2 = select();
			crossover(p1, p2, nextPopulation[i], nextPopulation[i + 1]);
		}
		if (i < POPUSIZE) {
			nextPopulation[i] = select();
		}
		//对所有新个体进行变异
		mutate();

		//精英保留
		for (int i = ELITISMNUMBER; i < POPUSIZE; i++) {
			population[i] = nextPopulation[i];
		}

		evaluate();
		elitism();

		generation++;
		cout << "Best individual in generation " <<generation << ": " << population[0].fitness << endl;
		if (population[0].fitness == maxFitness) {
			repeat++;
		}
		else {
			maxFitness = population[0].fitness;
			end = clock();
			repeat = 1;
		}
	}
	outputResult();
}

//返回[begin, end]范围内的一个随机整数
int GA::randInt(int begin, int end) {
	return rand() % (end - begin) + begin + 1;
}

//返回[0，1]的浮点数
double GA::randFloat() {
	return rand() / double(RAND_MAX);
}

//随机生成可行解
Individual GA::randomIndiv() {
	Individual newIndiv;
	do {
		for (int i = 0; i < csLen; i++) {
			if (determine[i] != -1) {
				newIndiv.chromosome[i] = determine[i];
			}
			else {
				newIndiv.chromosome[i] = (randFloat() > 0.5 ? 1 : 0);
			}
		}
	} while ((G.generateSubGraph(newIndiv.chromosome, csLen)).evaluate() == INF);
	return newIndiv;
}

//初始化种群
void GA::initialize() {
	//预先判断
	//1、如果度数为1，且非正则点，就一定不在Steiner最小树中
	//2、如果度数为1，且是正则点，与该点相邻的顶点一定在Steiner最小树中
	//3、如果点v度数为2，邻接点为x,y且xy相连，如果xv + yv > xy, 则点v一定不在Steiner最小树中
	int sum = 0;
	for (int i = 0; i < csLen; i++) {
		if (G.getDegree(i) == 1) {
			if (G.terminals[i] == 0) {
				determine[i] = 0;
			}
			else {
				for (int j = 0; j < csLen; j++) {
					if (G.matrix[i][j] != 0) {
						determine[j] = 1;
						break;
					}
				}
			}
		}
		if (G.getDegree(i) == 2 && G.terminals[i] == 0) {
			int node[2], k = 0;
			for (int j = 0; j < csLen; j++) {
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

	cout << "Please wait a minute， we are prepare the initial population for GA..." << endl;
	for (int i = 0; i < POPUSIZE; i++) {
		//cout << i << endl;
		population[i] = randomIndiv();
	}
}

//评价
void GA::evaluate() {
	for (int i = 0; i < POPUSIZE; i++) {
		population[i].fitness = (G.generateSubGraph(population[i].chromosome, csLen)).evaluate();
	}
}

//筛选出精英个体（按fitness排序）
void GA::elitism() {
	sort(population, population + POPUSIZE - 1, [](Individual indiv1, Individual indiv2)
	{return indiv1.fitness < indiv2.fitness; });
}

//锦标赛选择
Individual GA::select() {
	int pos;
	int bestfit = INF + 1;
	for (int i = 0; i < TOURNAMENTSIZE; i++) {
		int r = randInt(0, POPUSIZE - 1);
		if (population[r].fitness < bestfit) {
			pos = r;
			bestfit = population[r].fitness;
		}
	}
	return population[pos];
}

//两点交叉
void GA::crossover(const Individual& p1, const Individual& p2, Individual& baby1, Individual& baby2) {
	baby1 = p1;
	baby2 = p2;

	double p = randFloat();
	if (p > CROSSEROVER_RATE)
		return;

	int point1 = randInt(1, csLen - 2);
	int point2 = randInt(1, csLen - 2);
	if (point2 < point1) {
		swap(point1, point2);
	}

	for (int i = 0; i < csLen - 1; i++) {
		if (i < point2 && i > point1)
			continue;
		baby1.chromosome[i] = p2.chromosome[i];
		baby2.chromosome[i] = p1.chromosome[i];
	}
	//若交叉产生了无效解，换原
	if ((G.generateSubGraph(baby1.chromosome, csLen)).evaluate() == INF)
		baby1 = p1;
	if ((G.generateSubGraph(baby2.chromosome, csLen)).evaluate() == INF)
		baby2 = p2;
}

//变异
void GA::mutate() {
	double x;
	for (int i = ELITISMNUMBER; i < POPUSIZE; i++) {
		Individual temp = nextPopulation[i];

		for (int j = 0; j < csLen; j++) {
			if (G.terminals[j])
				continue;
			x = randFloat();
			if (x < MUTATION_RATE) {
				nextPopulation[i].chromosome[j] ^= 1;
			}
		}
		//如果变异后成了无效解，那么取消变异
		if ((G.generateSubGraph(nextPopulation[i].chromosome, csLen)).evaluate() == INF)
			nextPopulation[i] = temp;
	}
}

void GA::outputResult()
{
	cout << "We finished! the optimal minimum value is " << population[0].fitness << endl;
	cout << "The steiner point is:";
	for (int i = 0; i < csLen; i++) {
		if (population[0].chromosome[i] == 1 && G.terminals[i] == 0) {
			cout << " " << i + 1;
		}
	}
	cout << endl;
	cout << "Time spent from begin to the optimal value found: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
}
