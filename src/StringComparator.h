#pragma once

#include "stdafx.h"
#include "StringComparatorState.h"

class StringComparator {

private:

	StringComparatorState* state;

public:

	StringComparator();
	virtual ~StringComparator();

	bool compare(const char* obj1, const char* obj2);
	
	int comparePart(const char* obj1, const char partObj2);

};