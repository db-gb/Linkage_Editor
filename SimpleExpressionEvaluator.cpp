#include "SimpleExpressionEvaluator.h" 


SimpleExpressionEvaluator::SimpleExpressionEvaluator(string operandStr)
{
	symbol1 = "";
	symbol2 = "";
	operation = "";
	bool symbol1Complete = false;
	bool symbol2Complete = false;
	bool operationObtained = false;
	NumSymbols = 0;

	int i;
	string token = "";

	for (i = 0; i < operandStr.length(); i++)
	{
		if (iswspace(operandStr[i]) || operandStr[i] == '#' || operandStr[i] == ',')
		{
			if (symbol1.length() > 0 && !symbol1Complete)
			{
				symbol1Complete = true;
			}
			if (symbol2.length() > 0 && !symbol2Complete)
			{
				symbol2Complete = true;
			}

			continue;
		}
		if (operandStr[i] != '+' && operandStr[i] != '-')
		{
			if (!symbol1Complete)
			{
				symbol1.push_back(operandStr[i]);
			}
			else if (!symbol2Complete)
			{
				symbol2.push_back(operandStr[i]);
			}
		}
		if ((operandStr[i] == '+' || operandStr[i] == '-') && symbol1.length() > 0)
		{
			if (!operationObtained)
			{
				symbol1Complete = true;
				operation.push_back(operandStr[i]);
				operationObtained = true;
			}
			else
			{
				printf("Syntax error when parsing arithmetic expression!\n");
			}
		}

		if (operandStr[i] == ',')
		{
			if (symbol1.length() > 0 && !symbol1Complete)
			{
				symbol1Complete = true;
			}
			else if (symbol2.length() > 0 && !symbol2Complete)
			{
				symbol2Complete = true;
			}
		}

	}

	if (symbol1Complete)
		NumSymbols++;
	if (symbol2Complete)
		NumSymbols++;


}