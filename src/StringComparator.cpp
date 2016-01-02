
#include "stdafx.h"
#include "StringComparator.h"



StringComparator::StringComparator()
{
	this->state = new StringComparatorState();
	this->state->done = false;
	this->state->error = false;
	this->state->comparePos = 0;
	this->state->obj1 = "";
}

StringComparator::~StringComparator()
{
	this->state = nullptr;
}

bool StringComparator::compare(const char* obj1, const char* obj2)
{
	return strcmp(obj1,obj2) == 0;
}

int StringComparator::comparePart(const char* obj1, const char partObj2)
{
	int result = 1;
	if(strcmp(this->state->obj1.c_str(),obj1) != 0)
	{
		this->state->obj1 = obj1;
	}
	int length = strlen(obj1);
	char tmp = obj1[this->state->comparePos];
	int v1 = static_cast<int>(obj1[this->state->comparePos]);
	int v2 = static_cast<int>(partObj2);
	if(obj1[this->state->comparePos] == partObj2)
	{
		this->state->comparePos +=1;
		this->state->error = false;
		if(strlen(obj1) == this->state->comparePos)
		{
			// comparison done
			result = 2;
		}
		else
		{
			// comparison of char ok, but not completly solved
			result = 3;
		}
	}
	else
	{
		// comparison failure. reset compare pos
		this->state->comparePos = 0;
		this->state->error = true;
	}

	return result;
}