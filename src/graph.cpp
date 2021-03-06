#include "graph.h"


std::string remove_wspace(const std::string& str)
{
	// function to remove whitespaces from string
	std::string tempstr;
	for (char const &c : str)
	{
		if (c==' ') continue;
		else tempstr+=c;
	}
	return tempstr;
}

bool isNumber(const std::string& str)
{
	// function to determine if a string is an int
    for (char const &c : str) 
    {
        if (isdigit(c) == 0) return false;
    }
    return true;
}

// constant map to store operator priority
std::unordered_map<std::string, int> const OPS_PRIORITY(
{
	{"^", 4},
	{"*", 3},
	{"/", 2},
	{"+", 1},
	{"-", 0}
});

// helper function to cover each operation
float calculate(float a, float b, std::string op)
{
	if (op == "^")
		return pow(a, b);
	else if (op =="*")
		return a*b;
	else if (op == "/")
		return a/b;
	else if (op == "+")
		return a+b;
	else if (op == "-")
		return a-b;
	else
		return 0;
}

// operator wrapper functions //
float add(float a, float b)
{
	return a+b;
}

float sub(float a, float b)
{
	return a-b;
}

float mul(float a, float b)
{
	return a*b;
}

float div(float a, float b)
{
	return a/b;
}


// Equation class functions //
Equation::Equation(std::string equation)
{
    bool is_digit = false;
    bool found_token = false;
    std::string token = "";
    std::stack<std::string> ops;

    // get implicit eqn
    int eqId = equation.find("=");
    imp_exp = equation.substr(0, eqId)+"-("+equation.substr(eqId+1, equation.length()-eqId-1)+")";
    imp_exp = remove_wspace(imp_exp);

    for (int i = 0; i<(int)imp_exp.length(); i++)
    {
    	// get token
    	if (token=="")
    	{
    		if (imp_exp[i] == 'x' || imp_exp[i] == 'y')
    		{
    			token += imp_exp[i];
    			found_token = true;
    			is_digit = true;
    		}
    		else if (isdigit(imp_exp[i]))
    		{
    			is_digit = true;
    			token += imp_exp[i];
    		}
    		else
    		{
    			token = imp_exp[i];
    			found_token = true;
    		}
    	}
    	else if (token!="" && is_digit)
    	{
    		if (isdigit(imp_exp[i]))
    		{
    			token += imp_exp[i];
    			if (i == (int)imp_exp.length()-1)
    			{	// no more chars left to parse
    				found_token = true;
    			}
    		}
    		else  // num found
    		{
    			// set index back so the next token
    			// (operator) does not get ignored
    			i--;
    			found_token = true;
    		}
    	}

		// put token in stack/queue
    	if (found_token)
    	{
    		if (is_digit)
    		{
    			rev_pol.push_back(token);
    			// reset is_digit after token ahs been processed
		    	is_digit = false;
		    }
    		else if (token == "(")
    			ops.push(token);
    		else if (token == ")")
    		{
    			while (ops.top() != "(")
    			{
    				// push operator to reverse polish queue
    				rev_pol.push_back(ops.top());
    				// pop operator from stack
    				ops.pop();
    			}
    			ops.pop();  // pop "("
    		}
    		else	// all other operators
    		{
    			if (ops.empty() || 
    				ops.top() == "(" || 
    				OPS_PRIORITY.at(token) > OPS_PRIORITY.at(ops.top()))
    			{
    				// higher priority gets pushed automatically to the top
    				ops.push(token);
    			}
    			else
    			{
    				while (!ops.empty() && ops.top() != "(" && OPS_PRIORITY.at(token) < OPS_PRIORITY.at(ops.top()))
    				{
    					// push operator to queue
    					rev_pol.push_back(ops.top());
    					ops.pop();
    				}
    				ops.push(token);
    			}

    		}

    		// reset token
   			token = "";
   			found_token = false;
    	}
    }

    // parse stack
    while (!ops.empty())
    {
    	rev_pol.push_back(ops.top());
    	ops.pop();
    }

    this->equation = equation;

    // print reverse polish
    std::cout << '\n';
    for (auto x: rev_pol)
    	std::cout << x << std::endl;
}

std::vector<float> Equation::parse_point(float x, float y, float scale)
{
	x=x/scale, y=y/scale;

    std::vector<float> point;
	std::stack<float> output_stack;
	for (auto token : rev_pol)
	{
		if (!isNumber(token) && token != "x" && token != "y")
		{
			float operand2 = output_stack.top();
			output_stack.pop();
			float operand1 = output_stack.top();
			output_stack.pop();

			output_stack.push(calculate(operand1, operand2, token));
		}
		else
		{
			if (token == "x")
				output_stack.push(x);
			else if (token == "y")
				output_stack.push(y);
			else
				output_stack.push((float)std::stoi(token));
		}
	}

	float val = output_stack.top();
    if (val <= 1)
    {
        point.push_back(1.0f);
        point.push_back(val);
        return point;
    }
    else
    {
        point.push_back(0.0f);
        point.push_back(val);
        return point;
    }
}