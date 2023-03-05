/*
 * Arc.cpp
 *
 *  Created on: 16 Apr 2022
 *      Author: hobbes
 */

#include "entities/Arc.h"

namespace dxf {

using std::to_string;

Arc::Arc(const vector<Group> &properties) :Entity(properties) {

	for (unsigned int i = 0; i < properties.size(); ++i) {

		Group g = properties[i];
		switch (g.groupcode) {
			// 100 Subclass marker (AcDbLine) - read in Entity
			case 39:	// Thickness (optional; default = 0)
				this->thicness_ = stod(g.value);
				break;
			case 10:	// Start point (in WCS)
				this->centerPoint_.x_ = stod(g.value);
				break;
			case 20:
				this->centerPoint_.y_ = stod(g.value);
				break;
			case 30:
				this->centerPoint_.z_ = stod(g.value);
				break;
			case 40:
				this->radius_ = stod(g.value);
				break;
			case 50:
				this->startAngle_ =  stod(g.value);
				break;
			case 51:
				this->endAngle_ =  stod(g.value);
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

Arc::~Arc() {
	// TODO Auto-generated destructor stub
}

string Arc::toJson() {
	string s;
	s += "{\"arc\" : {";
		s += toString();

		s += ", \"center_point\" : {" ;
		s += "\"x\" : " + to_string(this->centerPoint_.x_) + ", ";
		s += "\"y\" : " + to_string(this->centerPoint_.y_) + ", ";
		s += "\"z\" : " + to_string(this->centerPoint_.z_) + "}";

		s += ", \"start_angle\" : "  + to_string(this->startAngle_) + ", ";
		s += ", \"start_angle\" : "  + to_string(this->endAngle_) + "} ";
		s += "}";
		s += "}";
	return s;
}

} /* namespace dxf */
