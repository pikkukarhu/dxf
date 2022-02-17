/*
 * Line.cpp
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#include "Line.h"

namespace dxf {

Line::Line(const vector<Group> &properties) :Entity(properties) {
	// TODO Auto-generated constructor stub
	// Read end & start from groups. (Other line spacific) Entity should read directly
}

Line::~Line() {
	// TODO Auto-generated destructor stub
	// Probably nothing
}

} /* namespace dxf */
