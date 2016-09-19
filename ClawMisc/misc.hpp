#ifndef _STL_MISC_HPP_
#define _STL_MISC_HPP_

// ------------------------------------------
// ------ PARSING TOOLS AND ERROR LOGS ------
// ------------------------------------------
#include <list>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include "text.hpp"
#include "resourcehandler.hpp"

//#define USE_EXCEPTIONS


struct MyException : public std::exception {
	std::string s;
	MyException(std::string ss) :
		s(ss)
		{}
	const char* what() const throw() {return s.c_str();}
};

struct MyWarning : public std::exception {
	std::string s;
	MyWarning(std::string ss) :
		s(ss)
		{}
	const char* what() const throw() {return s.c_str();}
};


extern void toErrorLog(const std::string& msg);
extern void toWarningLog(const std::string& msg);
extern void toInitLog(const std::string& msg);
extern void toInitLog(const Resource& msg);
extern std::vector<std::string> findFiles(const std::string& directory1, const std::string& directory2 = "", const std::string& directory3 = "", const std::string& directory4 = "", const std::string& directory5 = "", const std::string& directory6 = "");
extern void loadStringFile(const std::string& string_file, const std::string file_extension, const std::string& file_type_identifier, ResourceHandler& string_list);
extern void checkStreamIsOpen(const std::ifstream& stream, const std::string function_name, const std::string& file_name);
extern void checkStreamIsOpen(std::ofstream& stream, const std::string function_name, const std::string& file_name);
extern void checkStreamForFailure(const std::ifstream& stream, const std::string function_name, const std::string& file_name);
extern void checkStreamForFailure(std::ofstream& stream, const std::string function_name, const std::string& file_name);

extern const bool checkIgnoreFileName(const std::string file_name, const std::string extension);

extern const bool parse_line(const std::string& text, std::list<std::string>& words);
extern const bool parse_block_map(std::ifstream& stream, std::map<std::string, std::list<std::string> >& block);
extern const bool parse_block_list(std::ifstream& stream, std::list<std::list<std::string> >& block);
//extern void parse_2nd_block(std::ifstream& stream, std::map<std::string, std::map<std::string, std::list<std::string> > >& block);
extern const std::string formatTime(const int time, const unsigned int speed);
extern int my_round(double x);
// -----------------------------------------------
// ------  END PARSING TOOLS AND ERROR LOGS ------
// -----------------------------------------------


#endif

