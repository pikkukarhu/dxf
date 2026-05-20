/*
 * TableFactory.h
 *
 *  Created on: 10 Mar 2022
 *      Author: hobbes
 */
#pragma once

#ifndef TABLES_TABLEFACTORY_H_
#define TABLES_TABLEFACTORY_H_

#include <vector>
#include <map>
#include <string>

#include "file/File.h"
#include "TableEntry.h"

using std::vector;
using std::map;
using std::string;

namespace dxf {

class TableFactory final {
private:
	map<string, TableEntry* (*)(const vector<Group> &)> factories_;

public:
	TableFactory();
	virtual ~TableFactory();

	TableEntry* create(const vector<Group> &properties);
};

} /* namespace dxf */

#endif /* TABLES_TABLEFACTORY_H_ */
