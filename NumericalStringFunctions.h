#pragma once
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

unsigned int ConvertHexStringToNumber(string);

unsigned int ConvertDecStringToNumber(string);

bool IsEmptyString(string);

void UnitTestHexStringToNumConverter();

void UnitTestDecStringToNumConverter();

void UnitTestIsEmptyString();
