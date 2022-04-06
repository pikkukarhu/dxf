/*
 * Table.cpp
 *
 *  Created on: 25 Feb 2022
 *      Author: hobbes
 */

#include "tables/Table.h"


#include <vector>
#include <string>

#include "file/File.h"

using std::stoi;

namespace dxf {

Table::Table(const vector<Group> &properties) {

	for (unsigned int i = 0; i < properties.size(); ++ i) {
		Group g = properties[i];
		switch (g.groupcode) {
		case 0:			// 0	Object type (TABLE)
			this->objectType_ = g.value;
			break;
		case 2:
			this->tableName_ = g.value;		// 2	Table name
			break;
		case 5:
			this->handle_ = g.value;			// 5	Handle
			break;
		case 330:
			this->ownerSOftPtr_ = g.value;	// 330	Soft-pointer ID/handle to owner object
			break;
		case 100:
			this->subclassMarker_ = g.value;	// 100	Subclass marker (AcDbSymbolTable)
			break;
		case 70:
			this->entriesMaxNro_ = stoi(g.value);		// 70	Maximum number of entries in table
			break;
		}
	}

}

Table::~Table() {
	// TODO  Debug -- print this table.
	for (unsigned int i = 0; i < this->entryes_.size(); ++i) {

		delete this->entryes_[i];
	}
}

string Table::toString() {
	string s;
	s += 	"\"object_type\" : \"" + this->objectType_ + "\", ";
	s +=   "\"table_name\" : \"" + this->tableName_ + "\", ";
	s +=   "\"handle\" : \"" + this->handle_ + "\", ";
	s +=   "\"owner_soft_pointer\" : \"" + this->ownerSOftPtr_ + "\", ";
	s +=   "\"subclass_marker\" : \"" + this->subclassMarker_ + "\", ";
	s +=   "\"subclass_marker\" : \"" + this->subclassMarker_ + "\"";
	return s;
}

string Table::toJson() {
	string s = "{" + toString() + ", \"entries\" : [";
	for (int i = 0; i <  this->entryes_.size(); ++i) {
		s += this->entryes_[i]->toJson();
	}
	s += "]}";
	return s;
}


} /* namespace dxf */
