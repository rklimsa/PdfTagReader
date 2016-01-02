
#include "stdafx.h"
#include "FileHandler.h"
#include "StringComparator.h"

#include <iostream>

FileHandler::FileHandler(const char* filepath)
{
	this->cache = false;
	this->filepath = filepath;
	this->fileStream = new std::fstream(this->filepath,std::ios_base::binary | std::ios_base::in);
	this->sComparator = new StringComparator();
	this->ss.str("");
}

FileHandler::~FileHandler()
{
	
}

size_t FileHandler::read(const size_t pos, const char* token, const int seekway)
{
	return read(pos,token,seekway,nullptr);
}

size_t FileHandler::read(const size_t pos, const char* beginToken, const char* endToken, const int seekway, std::hash_map<std::string,std::string>* params)
{
	this->cache = false;
	const size_t begPos = read(pos,beginToken,seekway);
	this->cache = true;
	return read(begPos,endToken,seekway,params);
}

size_t FileHandler::read(const size_t pos, const char* token,const int seekway, std::hash_map<std::string, std::string>* params)
{
	if(this->fileStream->bad())
		return -1;

	if(this->fileStream->eof())
		return -2;

	if(pos < 0)
		return -4;

	setReadingPosition(pos,seekway);
	size_t curPos = this->fileStream->tellg();
	size_t res = readUntilToken(token);
	if(params==nullptr)
	{
		return res;
	}	
	else
	{
		setReadingPosition(curPos,seekway);
		this->ss.str("");
		return readParams(res,token,params);	
	}

}

void FileHandler::setReadingPosition(const size_t pos, const int seekway)
{
	this->fileStream->seekg(pos,seekway);
}

size_t FileHandler::readUntilToken(const char* token)
{
	int result = 0;

	while(result != 2)
	{
		const char c = this->fileStream->get();
		result = processChar(token,c);
		writeIntoCache(c);		
	}
	return this->fileStream->tellg();
}

int FileHandler::processChar(const char* token, const char c)
{
	int result = this->sComparator->comparePart(token,c);
	switch (result)
	{
	case 1:
		//std::cout<<"Wrong comparison"<<std::endl;
		break;
	case 2:
	{
		std::cout<<"Correct comparison. Done. Pos: "<<this->fileStream->tellg()<<std::endl;

		// Lösche letztes token aus gecachtem String
		if(this->cache == true && strlen(token) > 1 && this->ss.str().empty() == false)
		{
			size_t size = this->ss.str().size() - strlen(token);
			char* buffer = new char[size];
			this->ss.read(buffer,size);
			this->ss.str("");
			this->ss.write(buffer,size);		
		}
		break;		
	}
	case 3:
		std::cout<<"Correct comparison. In progress."<<std::endl;
		break;
	default:
		std::cout<<"Unsupported behavior. result: "<<result<<std::endl;
		break;
	}
	return result;
}

size_t FileHandler::readParams(const size_t posToRead, const char* token, std::hash_map<std::string, std::string>* params)
{
	std::string key = "";
	std::string value = "";
	std::string prefix = "";
	bool skip = false;
	std::string tmp = "";

	while(this->fileStream->tellg() < posToRead) 
	{
		const char c = this->fileStream->get();
		//Löse innere Objekte ("Dictionaries") auf
		if(c == '<' && this->fileStream->peek() == 0x3C)
		{
			if(this->ss.str().empty() == false)
			{
				prefix = this->ss.str();
				this->ss.str("");
			}
			skip = true;
			continue;
		}

		if(skip == true)
		{
			skip = false;
			continue;
		}

		if(
			processChar("/",c) == 2 
			|| processChar("//",c) == 2  
			|| processChar("\r",c) == 2  
			|| processChar("\n",c) == 2 
			|| processChar(">",c) == 2
			|| (processChar(" ",c) == 2 && !(this->fileStream->peek() == 0x30 /* "0" */ || this->fileStream->peek() == 0x52 /* "R" */ ))
			|| processChar("]",c) == 2 
			|| processChar(")",c) == 2
			|| processChar("(",c) == 2
			|| processChar("[",c) == 2
		)
		{
			if(this->ss.str().empty() == false)
			{
				if(key.size() == 0)
				{
					if(prefix.empty() == false)
					{
						tmp = this->ss.str();
						this->ss.str("");
						this->ss << prefix << "-" << tmp;					
					}
					key = this->ss.str();					
					this->ss.str("");
				} 
				else
				{
					value = this->ss.str();
				}
			}
		}
		else
		{
			writeIntoCache(c);
		}
		if(key.size() > 0 && value.size() > 0)
		{
			params->insert(std::make_pair(key,value));
			key = "";
			value = "";
			this->ss.str("");
		}	
	}

	return posToRead;
}

void FileHandler::setCache(const bool cache)
{
	this->cache = cache;
}

bool FileHandler::isCache()
{
	return this->cache;
}

std::string FileHandler::getCachedString()
{
	return this->ss.str();
}

void FileHandler::writeIntoCache(const char c)
{
	if(this->cache == true)
	{
		this->ss << c;	
	}
}