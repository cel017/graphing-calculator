#ifndef _GRAPH_
#define _GRAPH_

#include <cmath>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>

// func to check if string is num
bool isNumber(const std::string& str);
// priority map for operators
extern std::unordered_map<std::string, int> const OPS_PRIORITY;
// calculate a<binary operator >b
float calculate(float a, float b, std::string op);

class Equation
{
public:
	Equation(std::string equation);
	std::vector<float> parse_point(int x, int y, float scale = 25.0f);
private:
	std::string equation;
	std::string imp_exp;  // implicit expression
	std::vector<std::string> rev_pol;   // reverse polish notation
};

#endif
