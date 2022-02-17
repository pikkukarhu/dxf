/*
 * Document.h
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef DOCUMENT_DOCUMENT_H_
#define DOCUMENT_DOCUMENT_H_

#include <vector>
#include <fstream>

#include "../entities/Entity.h"
#include "../entities/Block.h"
#include "../header/Variable.h"

using std::vector;

namespace dxf {

class Document final {
private:
	string file_;

	vector <Entity> entityes_;
	vector <Block>  blocks_;
	vector <Variable> variables_;

	typedef struct {
		int groupcode;
		string value;
	} Group_;

	std::istream& readGroup(std::istream& is, Group_& g);

public:
	inline Document(string fileName) {this->file_ = fileName; }
	virtual ~Document() {}

	int readFile();

};

} /* namespace dxf */

#endif /* DOCUMENT_DOCUMENT_H_ */
