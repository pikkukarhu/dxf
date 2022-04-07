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


using std::vector;
using std::string;

namespace dxf {

class Document final {
private:
//	vector<Entity* > entities_;
	Entities entities_;
//	vector<Table *> tables_;
	Tables tables_;


	void createEntity(const vector<string>& props);
	Table* createTable(const vector<string>& props);

public:
	Document(string file);
	virtual ~Document();


};

} /* namespace dxf */

#endif /* DOCUMENT_DOCUMENT_H_ */
