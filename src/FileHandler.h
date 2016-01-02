#pragma once

#include "stdafx.h"
#include "StringComparator.h"

#include <hash_map>
#include <fstream>
#include <sstream>
#include <string>

class FileHandler {

private:
	const char* filepath;
	std::fstream* fileStream;
	StringComparator* sComparator;
	bool cache;
	std::stringstream ss;


	void setReadingPosition(const size_t pos, const int seekway);

	size_t readUntilToken(const char* token);

	size_t readParams(const size_t posToRead, const char* token,std::hash_map<std::string, std::string>* params);

	int processChar(const char* token, const char c);

	void writeIntoCache(const char c);

public:

	FileHandler(const char* filepath);

	virtual ~FileHandler();

	size_t read(const size_t pos, const char* token, const int seekway);

	size_t read(const size_t pos, const char* beginToken, const char* endToken, const int seekway, std::hash_map<std::string, std::string>* params);

	size_t read(const size_t pos, const char* token, const int seekway, std::hash_map<std::string,std::string>* params);

	void setCache(const bool cache);

	bool isCache();

	std::string getCachedString();

	
};