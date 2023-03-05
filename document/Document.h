/*
 * Entities.h
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef DOCUMENT_DOCUMENT_H_
#define DOCUMENT_DOCUMENT_H_

#include <vector>
#include <string>

#include "Entity.h"
#include "entities/Entities.h"
#include "file/File.h"
#include "tables/Tables.h"
#include "tables/Table.h"
#include "blocks/Blocks.h"


using std::vector;
using std::string;

namespace dxf {

class Document final {
private:
	Entities entities_;
	Tables tables_;
	Blocks blocks_;

	void createEntity(const vector<string>& props);
	Table* createTable(const vector<string>& props);

public:
	Document(string file);
	virtual ~Document();


};

} /* namespace dxf */

#endif /* DOCUMENT_DOCUMENT_H_ */
