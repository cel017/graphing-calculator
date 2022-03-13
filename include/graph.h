#ifndef _GRAPH_
#define _GRAPH_

#include <cmath>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>

// priority map for operators
extern std::unordered_map<std::string, int> const OPS_PRIORITY;
// calculate a<binary operator >b
float calculate(float a, float b, std::string op);

class Equation
{
public:
	Equation(std::string equation);
private:
	std::string equation;
	std::string imp_exp;  // implicit expression
	std::queue<std::string> rev_pol;   // reverse polish notation
};

#endif
