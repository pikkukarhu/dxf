/*
 * TableEntry.cpp
 *
 *  Created on: 25 Feb 2022
 *      Author: hobbes
 */

#include "tables/TableEntry.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace dxf {

TableEntry::TableEntry(const vector<Group> &properties) {

	for (unsigned int i = 0; i < properties.size(); ++ i) {
		Group g = properties[i];
		switch (g.groupcode) {
		case 0:			// 0   : Entry type (table name)
			this->entryType_ = g.value;
			break;
		case 5:			// 5   : Handle (all except DIMSTYLE)
		case 105:		// 105 : Handle (DIMSTYLE table only)
			this->handle_ = g.value;
			break;
		case 330:		// 330	: Soft-pointer ID/handle to owner object
			this->ownerHandle_ = g.value;
			break;
		case 100:		// 100	: Subclass marker (AcDbSymbolTableRecord)
			this->subclassMarker_ = g.value;
			break;
		}
	}
}

TableEntry::~TableEntry() {
	// TODO Auto-generated destructor stub
}

string TableEntry::toString() {
	string s;
	s += "\"entry_type\" : \"" + this->entryType_ +"\", ";
	s += "\"handle\" : \"" + this->handle_ +"\", ";
	s += "\"owner_handle\" : \"" + this->ownerHandle_ +"\", ";
	s += "\"subclass_marker\" : \"" + this->subclassMarker_ +"\"";

	return s;

}

} /* namespace dxf */
