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
	std::ifstream* is_;

public:
	File(string fileName);
	virtual ~File();

	std::istream& readGroup(std::istream& is, Group& g);
	bool readGroup(Group& g);

	// TODO test,remove later, document reads file.
	int readFile();
};

} /* namespace dxf */

#endif /* FILE_FILE_H_ */
