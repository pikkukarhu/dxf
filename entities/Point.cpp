/*
 * Point.cpp
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#include "Point.h"

#include <vector>
#include <string>

#include "document/Entity.h"
#include "file/File.h"

using std::vector;
using std::string;

namespace dxf {

Point::Point(const vector<Group> &properties) :Entity(properties) {
	this->pt_.x_ = 0.0;
	this->pt_.y_ = 0.0;
	this->pt_.z_ = 0.0;
	// TODO Auto-generated constructor stub

}

Point::~Point() {
	// TODO Auto-generated destructor stub
}

} /* namespace dxf */
