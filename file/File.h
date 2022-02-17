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

#include "../document/Block.h"
#include "../document/Entity.h"
#include "../header/Variable.h"

using std::vector;

namespace dxf {

struct Group {
	int groupcode;
	string value;
};

class Document final {
private:
	string file_;

	vector <Entity> entityes_;
	vector <Block>  blocks_;
	vector <Variable> variables_;

	std::istream& readGroup(std::istream& is, Group& g);

public:
	inline Document(string fileName) {this->file_ = fileName; }
	virtual ~Document() {}

	int readFile();

};

} /* namespace dxf */

#endif /* FILE_FILE_H_ */
