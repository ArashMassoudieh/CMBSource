
#pragma once
#include <vector>

//using namespace std;
class CIndividual  
{
public:
	CIndividual();
	CIndividual(int n);
	CIndividual(const CIndividual &C);
    CIndividual operator = (const CIndividual &C);
	virtual ~CIndividual();
	std::vector<double> x;
	std::vector<double> pert;
	std::vector<int> dir;
	std::vector<double> perteff;
	double fitness;
	double actual_fitness, actual_fitness2;
	int parent1, parent2, xsite;
	int nParams;
	std::vector<int> precision;
	std::vector<double> minrange, maxrange;
    void initialize();
    void mutate(double mu);
	int rank;
    void shake(double shakescale);
    std::vector<double> fit_measures;

};


double GetRndUnif(double xmin, double xmax);
void cross(const CIndividual &I1, const CIndividual &I2, CIndividual &IR1, CIndividual &IR2);
void cross2p(const CIndividual &I1, const CIndividual &I2, CIndividual &IR1, CIndividual &IR2);
void cross_RC_L(const CIndividual &I1, const CIndividual &I2, CIndividual &IR1, CIndividual &IR2);

