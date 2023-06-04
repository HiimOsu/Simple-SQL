#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <iomanip>
#include "../error/error.h"
#include <fstream>

// using namespace std;

bool file_exists(const char filename[]);

void open_fileRW(std::fstream& f, const char filename[]) throw(const Error_msg);
void open_fileR(std::fstream& f, const char filename[]) throw(const Error_msg);
void open_fileW(std::fstream& f, const char filename[])throw(const Error_msg);

#endif