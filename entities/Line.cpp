/*
 * Line.cpp
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#include "Line.h"
#include "document/Entity.h"
#include "file/File.h"

namespace dxf {

using std::to_string;

Line::Line(const vector<Group> &properties) :Entity(properties) {
	// Read end & start from groups. (Other line spacific) Entity should read directly

	for (unsigned int i = 0; i < properties.size(); ++i) {

		Group g = properties[i];
		switch (g.groupcode) {
			// 100 Subclass marker (AcDbLine) - read in Entity
			case 39:	// Thickness (optional; default = 0)
				this->thicness_ = stod(g.value);
				break;
			case 10:	// Start point (in WCS)
				this->start_.x_ = stod(g.value);
				break;
			case 20:
				this->start_.y_ = stod(g.value);
				break;
			case 30:
				this->start_.z_ = stod(g.value);
				break;
			case 11:	// Endpoint (in WCS)
				this->end_.x_ = stod(g.value);
				break;
			case 21:
				this->end_.y_ = stod(g.value);
				break;
			case 31:
				this->end_.z_ = stod(g.value);
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

string Line::toJson() {
	string s;
		s += "{\"line\" : {";
		s += toString();

		s += ", \"start_point\" : {" ;
		s += "\"x\" : " + to_string(this->start_.x_) + ", ";
		s += "\"y\" : " + to_string(this->start_.y_) + ", ";
		s += "\"z\" : " + to_string(this->start_.z_) + "}";

		s += ", \"end_point\" : {" ;
		s += "\"x\" : " + to_string(this->end_.x_) + ", ";
		s += "\"y\" : " + to_string(this->end_.y_) + ", ";
		s += "\"z\" : " + to_string(this->end_.z_) + "}";
		s += "}";
		s += "}";
	return s;
}

} /* namespace dxf */
