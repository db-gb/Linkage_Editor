#pragma once
#include <string>

#include "SimpleExpressionEvaluator.h"

using namespace std;

class SimpleExpressionEvaluator
{
private:

public:
	int NumSymbols;
	string symbol1;
	string operation;
	string symbol2;

	SimpleExpressionEvaluator(string operandStr);

};