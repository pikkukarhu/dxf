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
#include <iconv.h>

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
	iconv_t cd_ = (iconv_t)-1;
	string current_codepage_ = "UTF-8";

public:
	File(string fileName);
	virtual ~File();

	std::istream& readGroup(std::istream& is, Group& g);
	bool readGroup(Group& g);

	void setCodepage(const string& codepage);
	string toUtf8(const string& input);

	// TODO test,remove later, document reads file.
	int readFile();
};

} /* namespace dxf */

#endif /* FILE_FILE_H_ */
