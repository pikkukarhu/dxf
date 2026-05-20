/*
 * Insert.cpp
 *
 *  Created on: 4 Feb 2024
 *      Author: hobbes
 */

#include "entities/Insert.h"

namespace dxf {

Insert::Insert(const vector<Group> &properties)   :Entity(properties) {

	/*
	 100 Subclass marker (AcDbBlockReference)
66 Variable attributes-follow flag (optional; default = 0); if the value of attributes-follow flag is 1,
a series of attribute entities is expected to follow the insert, terminated by a seqend entity
2 Block name
10 Insertion point (in OCS)
DXF: X value; APP: 3D point
20, 30 DXF: Y and Z values of insertion point (in OCS)
41 X scale factor (optional; default = 1)
42 Y scale factor (optional; default = 1)
	 */
}

std::string Insert::to_json() { return ""; }
void Insert::to_svg(pugi::xml_node& svg_node) {
};

Insert::~Insert() {}
}

