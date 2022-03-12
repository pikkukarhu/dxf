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
using std::to_string;

namespace dxf {

Point::Point(const vector<Group> &properties) :Entity(properties) {
	this->pt_.x_ = 0.0;
	this->pt_.y_ = 0.0;
	this->pt_.z_ = 0.0;
	// TODO Auto-generated constructor stub

	for (unsigned int i = 0; i < properties.size(); ++i) {

		Group g = properties[i];
		switch (g.groupcode) {
			case 10:	// Point location (in WCS) X value; APP: 3D point
				this->pt_.x_ = stod(g.value);
				break;
			case 20:
				this->pt_.y_ = stod(g.value);
				break;
			case 30:
				this->pt_.z_ = stod(g.value);
				break;
			case 39:	// Thickness (optional; default = 0)
				this->thicness_ = stod(g.value);
				break;
			case 210: // Extrusion direction (optional; default = 0, 0, 1)
				this->extrDir_.x_ = stod(g.value);
				break;
			case 220:
				this->extrDir_.y_ = stod(g.value);
				break;
			case 230:
				this->extrDir_.z_ = stod(g.value);
				break;
			default:
				break;
		}
	}

}

string Point::toJson() {
	string s;
	s += "{\"point\" : {";
	s += toString();

	s += ", \"coordinates\" : {" ;
	s += "\"x\" : " + to_string(this->pt_.x_) + ", ";
	s += "\"y\" : " + to_string(this->pt_.y_) + ", ";
	s += "\"z\" : " + to_string(this->pt_.z_) + "}";

	s += "}";
	s += "}";

	return s;
}

} /* namespace dxf */
