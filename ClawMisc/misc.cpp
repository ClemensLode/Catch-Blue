// ------------------------------------------
// ------ PARSING TOOLS AND ERROR LOGS ------
// ------------------------------------------
#include "misc.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <sys/types.h>

#include <algorithm>

#ifdef __linux__
#else 
#include <windows.h>
#include <tchar.h>
#endif


void toErrorLog(const std::string& msg) {
	std::cout << msg << "\n";
#ifdef USE_EXCEPTIONS
	throw MyException(msg);
#endif
}

void toWarningLog(const std::string& msg) {
	std::cout << msg << "\n";
#ifdef USE_EXCEPTIONS
	throw MyWarning(msg);
#endif	
}



void toInitLog(const std::string& msg)
{
	std::cout << msg << "\n";
}

void toInitLog(const Resource& msg)
{
	std::cout << msg.toString() << "\n";
}


std::vector<std::string> findFiles(const std::string& directory1, const std::string& directory2, const std::string& directory3, const std::string& directory4, const std::string& directory5, const std::string& directory6)
{
	std::vector<std::string> fileList;
	std::ostringstream base;
	std::string delim;
#ifdef __linux__
	delim = "/";
#elif WIN32
	delim = "\\";
#endif
	if(directory6!="") {
		base << directory1 << delim << directory2 << delim << directory3 << delim << directory4 << delim << directory5 << delim << directory6;
	} else if(directory5!="") {
		base << directory1 << delim << directory2 << delim << directory3 << delim << directory4 << delim << directory5;
	} else if(directory4!="") {
		base << directory1 << delim << directory2 << delim << directory3 << delim << directory4;
	} else if(directory3!="") {
		base << directory1 << delim << directory2 << delim << directory3;
	} else if(directory2!="") {
		base << directory1 << delim << directory2;
	} else if(directory1!="") {
		base << directory1;
	}

#ifdef __linux__
	DIR *dir;
	struct dirent *entry;
	if ((dir = opendir(base.str().c_str())) == NULL) {
		throw MyException("ERROR (findFiles()): Cannot open directory " + base.str());
	}
	else 
	{
		while ((entry = readdir(dir)) != NULL) {
			fileList.push_back(base.str() + delim + entry->d_name);
		}
		closedir(dir);
	}
#elif WIN32
 	WIN32_FIND_DATA dir;
	HANDLE fhandle;
	if ((fhandle = FindFirstFile(((base.str() + delim + "*.*").c_str()), &dir)) != INVALID_HANDLE_VALUE)
	{
		do {
			fileList.push_back(base.str() + delim + dir.cFileName);
		} while(FindNextFile(fhandle, &dir));
	} else {
		throw MyException("ERROR (findFiles()): Cannot load " + base.str() + ".");
	}
	FindClose(fhandle);
#endif

	return fileList;
}


void checkStreamIsOpen(const std::ifstream& stream, const std::string function_name, const std::string& file_name)
{
	if(!stream.is_open())
	{
		throw MyException("ERROR (" + function_name + "()): File " + file_name + " not found or cannot be opened.");
	}
}

void checkStreamIsOpen(std::ofstream& stream, const std::string function_name, const std::string& file_name)
{
	if(!stream.is_open())
	{
		throw MyException("ERROR (" + function_name + "()): Could not create file " + file_name + " (write protection? disk space?).");
	}
}

void checkStreamForFailure(const std::ifstream& stream, const std::string function_name, const std::string& file_name)
{
	if(stream.bad()) {
		throw MyException("ERROR (" + function_name + "()): Fatal error while accessing file " + file_name + " => canceling request.");
	}
	if(stream.fail()) {
//		stream.clear(stream.rdstate() & ~std::ios::failbit);
		throw MyException("WARNING (" + function_name + "()): Error while reading from file " + file_name + " => retrying.");
	}
}

void checkStreamForFailure(std::ofstream& stream, const std::string function_name, const std::string& file_name)
{
	if(stream.bad())
	{
		throw MyException("ERROR (" + function_name + "()): Fatal error while accessing file " + file_name + " => canceling request.");
	}
	if(stream.fail())
	{
//		stream.clear(stream.rdstate() & ~std::ios::failbit);
		throw MyException("WARNING (" + function_name + "()): Error while writing to file " + file_name + " => retrying.");
	}
}

const std::string getLanguageSubDataEntry(const std::string& item)
{
	if(item.empty() || item.at(0) != '@') {
		return "";
	} else {
		std::string t = item.substr(1);
		std::transform(t.begin(), t.end(), t.begin(), toupper);
		return t;
	}
}
const bool parse_line(const std::string& text, std::list<std::string>& words)
{
	size_t n = text.length();
	size_t stop;
	bool in_parentheses=false;
	// " gefunden? ignoriere alle Sonderzeichen bis naechstes "
	size_t start = text.find_first_not_of("\t ");
	while (start!=std::string::npos) 
	{
		if(text[start]=='\"')
		{
			in_parentheses=true;
			++start;
		}
	
		if(in_parentheses) {
			stop = text.find_first_of("\"", start);
		} else {
			stop = text.find_first_of(",\t\" =", start);
		}
		in_parentheses = false;
		if(stop==std::string::npos) {
			stop = n;
		}
		std::string word = text.substr(start, stop - start);
		if(word == "@END") {
			return false;
		}
		words.push_back(word);
	   
		start = text.find_first_not_of(",\t =", stop+1);
	}
	return(true);
}

const bool parse_block_list(std::ifstream& stream, std::list<std::list<std::string> >& block)
{
	char line[1024];
	std::string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail()) {
			stream.clear(stream.rdstate() & ~std::ios::failbit);
		}
		text = line;
		size_t start=text.find_first_not_of("\t\" ");
		if((start==std::string::npos)||(text.empty()) ||(text[0]=='#')||(text[0]=='\0')) {
			continue; // ignore line
		}
		std::list<std::string> words;
		if(!parse_line(text, words)) { 
// => @END was found
			return true;
		}
		block.push_back(words);
	}
// end of file but no @END was found!
	return(false);
}

// parse until the next "@END", if there is no @END => return false
const bool parse_block_map(std::ifstream& stream, std::map<std::string, std::list<std::string> >& block)
{
	char line[1024];
	std::string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail()) {
			stream.clear(stream.rdstate() & ~std::ios::failbit);
		}

		text = line;
		size_t start = text.find_first_not_of("\t\" ");
		if((start == std::string::npos)||(text.empty()) ||(text[0]=='#')||(text[0]=='\0')) {
			continue; // ignore line
		}
		
		std::list<std::string> words;
		if(!parse_line(text, words)) {
			return true;
		}

		std::string word = words.front();
		block.insert(std::pair<std::string, std::list<std::string> >(word, words));
	}
	return false;
}

/*
void parse_2nd_block(std::ifstream& stream, std::map<std::string, std::map<std::string, std::list<std::string> > >& block)
{
	char line[1024];
	std::string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail())
			stream.clear(stream.rdstate() & ~std::ios::failbit);
		text=line;
//		if(text.find("@END")!=std::string::npos) 
//			return;
		size_t start=text.find_first_not_of("\t ");
		if((start==std::string::npos)||(text.empty()) ||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
		std::map<std::string, std::list<std::string> > words;
		parse_block(stream, words);
		block.insert(std::pair<std::string, std::map<std::string, std::list<std::string> > > (text.substr(start), words));
	}
}*/


void loadStringFile(const std::string& string_file, const std::string file_extension, const std::string& file_type_identifier, ResourceHandler& string_list)
{
	if(checkIgnoreFileName(string_file, file_extension)) {
		return;
	}

	bool string_mode = false;
	bool language_mode = false;
	std::string current_language = "";

	std::ifstream pFile(string_file.c_str());

	checkStreamIsOpen(pFile, "loadStringFile", string_file);

	toInitLog("* Loading string file " + string_file + ".");

	bool found_any_language_block = false;
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		checkStreamForFailure(pFile, "loadStringFile", string_file);

		std::string text = line;
		size_t start = text.find_first_not_of("\t ");
		if((start == std::string::npos) || (text.empty()) || (text[0] == '#') || (text[0] == '\0')) {
			continue; // ignore line
		}
		size_t stop = text.find_first_of("\t ", start);
		if(stop == std::string::npos) {
			stop = text.size();
		}
		std::string index = text.substr(start, stop);
		if(!string_mode)
		{
			string_mode = (index == ("@" + file_type_identifier));
			if(!string_mode)
			{
				if(index == "@END") {
					pFile.close();
					throw MyException("ERROR (loadStringFile()): Lonely @END in file " + string_file + ".");
				}
				else {
					pFile.close();
					throw MyException("ERROR (loadStringFile()): Line '" + index + "' is outside a block but is not marked as comment in file " + string_file + ".");
				}
			} else {
				language_mode = true;
			}
		}  else
		if((language_mode) && (index == "@END"))
		{
			current_language = "";
			language_mode = false;
		} else		
		if(string_mode && (index == "@END")) {
			string_mode = false;
		} else
		if((language_mode) && (current_language.empty()))
		{
			current_language = getLanguageSubDataEntry(index);
			// TODO Garbagecollector
			if(current_language.empty()) {
				pFile.close();			
				throw MyException("ERROR (loadStringFile()): Invalid language entry '" + index + "' in file " + string_file + ".");
			} else if(string_list.contains(current_language)) {
				pFile.close();			
				throw MyException("ERROR (loadStringFile()): Language '" + index + "' already initialized in file " + string_file + ".");
			}
			else
			{
				string_list.addResource(current_language, new ResourceHandler());
				found_any_language_block = true;
			}
		}
		// => hat nur 1 Ebene => Position festgestellt!
		else if((language_mode) && (!current_language.empty()))
		{
			std::map<std::string, std::list<std::string> > block;
			pFile.seekg(old_pos);
			if(!parse_block_map(pFile, block)) {
				pFile.close();			
				throw MyException("ERROR (loadStringFile()): No concluding @END for " + file_type_identifier + " block was found in file " + string_file + ".");
			}

			ResourceHandler& string_list_list = (ResourceHandler&)(string_list.lookUpResource(current_language));

			for(std::map<std::string, std::list<std::string> >::iterator i = block.begin(); i != block.end(); i++) {
				if(i->second.size() < 2) {
					throw MyException("ERROR (loadStringFile()): No string assigned to identifier " + i->first + " in file " + string_file + ".");
				}
				i->second.pop_front();
				string_list_list.addResource(i->first, new Text(i->second.front()));
			}
			current_language = "";
		} 
		old_pos = pFile.tellg();
	} // end while

	if(!found_any_language_block) {
		pFile.close();			
		throw MyException("ERROR (loadStringFile()): No language block (@ENGLISH, @GERMAN etc.) was found in file " + string_file + ".");
	}
	pFile.close();
}

const bool checkIgnoreFileName(const std::string file_name, const std::string extension) 
{
	if(file_name.substr(file_name.size() - 1, file_name.size()) == "." || file_name.size() < 2 + extension.size()) {
		return true;
	}
	if(file_name.substr(file_name.size() - extension.size(), extension.size()) != extension) {
		return true;
	}
	return false;
}

const std::string formatTime(const int time, const unsigned int speed)
{
	std::ostringstream os;
	os.str("");
	if(time < 0) {
		os << "- ";
	}
	int my_time = time / (6+speed*3);
	os << my_time/60 << ":" << std::setfill('0') << std::setw(2) << my_time%60;
	return(os.str());
}

// -----------------------------------------------
// ------  END PARSING TOOLS AND ERROR LOGS ------
// -----------------------------------------------

int my_round (double x) {
  int i = (int) x;
//  if (x >= 0.0) {
    return ((x-i) >= 0.5) ? (i + 1) : (i);
//  } else {
//    return (-x+i >= 0.5) ? (i - 1) : (i);
//  }
}