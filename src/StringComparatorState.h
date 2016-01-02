#pragma once

#include "stdafx.h"
#include "ComparatorState.h"

#include <string>

struct StringComparatorState : public ComparatorState {

	std::string obj1;
	std::string obj2;
	size_t lengthObj1;
	size_t lengthObj2;
	int comparePos;



};