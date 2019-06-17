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
		//����ʱ������Ӣ����
		for (i = ELITISMNUMBER; i + 1 < POPUSIZE; i += 2) {
			Individual p1 = select();
			Individual p2 = select();
			crossover(p1, p2, nextPopulation[i], nextPopulation[i + 1]);
		}
		if (i < POPUSIZE) {
			nextPopulation[i] = select();
		}
		//�������¸�����б���
		mutate();

		//��Ӣ����
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

//����[begin, end]��Χ�ڵ�һ���������
int GA::randInt(int begin, int end) {
	return rand() % (end - begin) + begin + 1;
}

//����[0��1]�ĸ�����
double GA::randFloat() {
	return rand() / double(RAND_MAX);
}

//������ɿ��н�
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

//��ʼ����Ⱥ
void GA::initialize() {
	//Ԥ���ж�
	//1���������Ϊ1���ҷ�����㣬��һ������Steiner��С����
	//2���������Ϊ1����������㣬��õ����ڵĶ���һ����Steiner��С����
	//3�������v����Ϊ2���ڽӵ�Ϊx,y��xy���������xv + yv > xy, ���vһ������Steiner��С����
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

	cout << "Please wait a minute�� we are prepare the initial population for GA..." << endl;
	for (int i = 0; i < POPUSIZE; i++) {
		//cout << i << endl;
		population[i] = randomIndiv();
	}
}

//����
void GA::evaluate() {
	for (int i = 0; i < POPUSIZE; i++) {
		population[i].fitness = (G.generateSubGraph(population[i].chromosome, csLen)).evaluate();
	}
}

//ɸѡ����Ӣ���壨��fitness����
void GA::elitism() {
	sort(population, population + POPUSIZE - 1, [](Individual indiv1, Individual indiv2)
	{return indiv1.fitness < indiv2.fitness; });
}

//������ѡ��
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

//���㽻��
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
	//�������������Ч�⣬��ԭ
	if ((G.generateSubGraph(baby1.chromosome, csLen)).evaluate() == INF)
		baby1 = p1;
	if ((G.generateSubGraph(baby2.chromosome, csLen)).evaluate() == INF)
		baby2 = p2;
}

//����
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
		//�������������Ч�⣬��ôȡ������
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
