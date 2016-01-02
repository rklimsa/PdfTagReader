// PdfTagReader.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "FileHandler.h"

#include <Windows.h>
#include <string>
#include <iostream>

void help();

int main(int argc, char* argv[])
{
	if(argc < 13)
	{
		help();
		std::cout<<"argc : "<<argc<<std::endl;
		for(int i = 0; i < argc; i++)
		std::cout<<"argv : "<<argv[i]<<std::endl;
	}
	else
	{
		std::string filepath = "";
		std::string startTag = "";
		std::string endTag = "";
		std::hash_map<std::string, std::string>* params = new std::hash_map<std::string, std::string>();
		size_t pos = 0;
		int direction = 0;
		size_t offset = 0;
		bool param = false;
		for(int i = 0; i< argc; i++)
		{
			if(strcmp(argv[i],"-f")==0)
			{
				filepath = argv[i+1];
			}

			if(strcmp(argv[i],"-s")==0)
			{
				startTag = argv[i+1];
			}

			if(strcmp(argv[i],"-e")==0)
			{
				endTag = argv[i+1];
			}

			if(strcmp(argv[i],"-p")==0)
			{
				if(strcmp(argv[i+1],"true")==0)
				{
					param = true;	
				}
			}

			if(strcmp(argv[i],"-o")==0)
			{
				offset = atoi(argv[i + 1]);
			}

			if(strcmp(argv[i],"-d")==0)
			{
				if(strcmp(argv[i+1],"SET")==0)
				{
					direction = SEEK_SET;
				} 
				else if(strcmp(argv[i+1],"CUR")==0)
				{
					direction = SEEK_CUR;
				} 
				else if(strcmp(argv[i+1],"END")==0)
				{
					direction = SEEK_END;
				}
			}
		}

		FileHandler* f = new FileHandler(filepath.c_str());
		if(param == true)
		{
			pos = f->read(0,startTag.c_str(),endTag.c_str(),direction,params);
		}
		else
		{
			pos = f->read(0,startTag.c_str(),endTag.c_str(),direction,nullptr);
		}

		std::cout<<"Pos: "<<pos<<std::endl;
		std::cout<<"String: "<<f->getCachedString()<<std::endl;
		std::cout<<"Params-Size: "<<params->size()<<std::endl;

		for(std::hash_map<std::string,std::string>::iterator it = params->begin(); it != params->end(); it++)
		{
			std::cout<<"Key: " << it._Ptr->_Myval.first << ", Value: " << it._Ptr->_Myval.second << std::endl; 
		}

		delete params;	
	}

	system("PAUSE");
	return EXIT_SUCCESS;
}

void help()
{
	std::cout<<"Usage: pdfTagReader.exe -f <pdfFile> -s <starttag> -e <endtag> -p <true|false> -o <readingOffset> -d <SET|CUR|END>"<<std::endl;
}