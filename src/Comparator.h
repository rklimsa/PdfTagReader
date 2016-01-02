#pragma once

#include "stdafx.h"

class Comparator {

public:

	template<typename T>
	bool virtual compare<T obj1, T obj2);

};