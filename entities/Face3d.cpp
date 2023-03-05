/*
 * Face3d.cpp
 *
 *  Created on: 16 Apr 2022
 *      Author: hobbes
 */

#include "entities/Face3d.h"

using std::to_string;

namespace dxf {

Face3d::Face3d(const vector<Group> &properties)  :Entity(properties) {
	for (unsigned int i = 0; i < properties.size(); ++i) {

		Group g = properties[i];
		switch (g.groupcode) {
			case 10:	// Start point (in WCS)
				this->first_.x_ = stod(g.value);
				break;
			case 20:
				this->first_.y_ = stod(g.value);
				break;
			case 30:
				this->first_.z_ = stod(g.value);
				break;
			case 11:	// Endpoint (in WCS)
				this->second_.x_ = stod(g.value);
				break;
			case 21:
				this->second_.y_ = stod(g.value);
				break;
			case 31:
				this->second_.z_ = stod(g.value);
				break;
			case 12:	// Endpoint (in WCS)
				this->third_.x_ = stod(g.value);
				break;
			case 22:
				this->third_.y_ = stod(g.value);
				break;
			case 32:
				this->third_.z_ = stod(g.value);
				break;
			case 13:	// Endpoint (in WCS)
				this->fourth_.x_ = stod(g.value);
				break;
			case 23:
				this->fourth_.y_ = stod(g.value);
				break;
			case 33:
				this->fourth_.z_ = stod(g.value);
				break;
			case 70:
				this->invisible_edges_ = stoi(g.value);
				break;
			default:
				break;
		}
	}
}

Face3d::~Face3d() {
	// TODO Auto-generated destructor stub
}

string Face3d::toJson() {
	string s;
	s += "{\"3d_face\" : {";
		s += toString();

		s += ", \"first_point\" : {" ;
		s += "\"x\" : " + to_string(this->first_.x_) + ", ";
		s += "\"y\" : " + to_string(this->first_.y_) + ", ";
		s += "\"z\" : " + to_string(this->first_.z_) + "}";

		s += ", \"second_point\" : {" ;
		s += "\"x\" : " + to_string(this->second_.x_) + ", ";
		s += "\"y\" : " + to_string(this->second_.y_) + ", ";
		s += "\"z\" : " + to_string(this->second_.z_) + "}";

		s += ", \"third_point\" : {" ;
		s += "\"x\" : " + to_string(this->third_.x_) + ", ";
		s += "\"y\" : " + to_string(this->third_.y_) + ", ";
		s += "\"z\" : " + to_string(this->third_.z_) + "}";

		s += ", \"fourth_point\" : {" ;
		s += "\"x\" : " + to_string(this->fourth_.x_) + ", ";
		s += "\"y\" : " + to_string(this->fourth_.y_) + ", ";
		s += "\"z\" : " + to_string(this->fourth_.z_) + "}";

		s += "}";
		s += "}";
	return s;
}

} /* namespace dxf */
