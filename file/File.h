/*
 * Document.h
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef FILE_FILE_H_
#define FILE_FILE_H_

#include <vector>
#include <fstream>
#include <string>

using std::vector;
using std::string;

namespace dxf {

class Group {
public:
	int groupcode;
	string value;
};

class File final {
private:
	string file_;

	std::istream& readGroup(std::istream& is, Group& g);

public:
	inline File(string fileName) {this->file_ = fileName; }
	virtual ~File() {}

	int readFile();
};

} /* namespace dxf */

#endif /* FILE_FILE_H_ */
