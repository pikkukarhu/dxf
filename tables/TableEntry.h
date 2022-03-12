/*
 * TableEntry.h
 *
 *  Created on: 25 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef TABLES_TABLEENTRY_H_
#define TABLES_TABLEENTRY_H_

#include <string>
#include <vector>

#include "file/File.h"

using std::string;
using std::vector;

namespace dxf {

class TableEntry {
private:
	string entryType_;	// 0   : Entry type (table name)
	string handle_;		// 5   : Handle (all except DIMSTYLE)
						// 105 : Handle (DIMSTYLE table only)

	/* ************************************************************************
	 * Application specific not implemented, neither plans to implement.
	 *
	 * Application specific groups not read
	 *
	 *	102:	Start of application-defined group “{application_name”. For example, “{ACAD_REACTORS”
	 *			indicates the start of the AutoCAD persistent reactors group (optional) application-defined codes
	 *			Codes and values within the 102 groups are application defined (optional)
	 *	102:	End of group, “}” (optional)
	 *	102:	“{ACAD_REACTORS” indicates the start of the AutoCAD persistent reactors group.
	 *			This group exists only if persistent reactors have been attached to this object (optional)
	 *
	 *	330:	Soft-pointer ID/handle to owner dictionary (optional)
	 *
	 *	102:	End of group, “}” (optional)
	 *	102:	“{ACAD_XDICTIONARY” indicates the start of an extension dictionary group.
	 *			This group exists only if persistent reactors have been attached to this object (optional)
	 *
	 *	360:	Hard-owner ID/handle to owner dictionary (optional)
	 *
	 *	102:	End of group, “}” (optional)
	 **************************************************************************/

	string ownerHandle_;	// 330	: Soft-pointer ID/handle to owner object
	string subclassMarker_;	// 100	: Subclass marker (AcDbSymbolTableRecord)

public:
	TableEntry(const vector<dxf::Group> &properties);
	virtual ~TableEntry();
};

} /* namespace dxf */

#endif /* TABLES_TABLEENTRY_H_ */
