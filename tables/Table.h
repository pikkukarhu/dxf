/*
 * Table.h
 *
 *  Created on: 25 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef TABLES_TABLE_H_
#define TABLES_TABLE_H_


#include <vector>
#include <string>

#include "TableEntry.h"
#include "TableFactory.h"
#include "file/File.h"

using std::vector;
using std::string;

namespace dxf {

class Table final {
private:
	TableFactory tableFactory_;
	vector<TableEntry* > entryes_;

	string objectType_;		// 0	Object type (TABLE)
	string tableName_;		// 2	Table name
	string handle_;			// 5	Handle
	/*
		102 “{ACAD_XDICTIONARY” indicates the start of an extension dictionary group. This group exists only if persistent reactors have been attached to this object (optional)
		360	Hard owner ID/handle to owner dictionary (optional)
		102	End of group, “}” (optional)
	*/
	string ownerSOftPtr_;	// 330	Soft-pointer ID/handle to owner object
	string subclassMarker_;	// 100	Subclass marker (AcDbSymbolTable)
	int	entriesMaxNro_;		// 70	Maximum number of entries in table
public:
	Table(const vector<Group> &properties);
	virtual ~Table();

	inline string Type() { return this->objectType_; }
	inline void createEntry(const vector<Group> &properties) { this->entryes_.push_back(this->tableFactory_.create(properties));};
};

} /* namespace dxf */

#endif /* TABLES_TABLE_H_ */
