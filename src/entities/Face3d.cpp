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

void Face3d::calc_bounding_box() {
	// TODO
}

string Face3d::to_json() {
	string s;
	s += "{\"3d_face\" : {";
		s += to_string();
		s += ", \"first_point\" : " + this->first_.as_string();
		s += ", \"second_point\" : " + this->second_.as_string();
		s += ", \"third_point\" : " + this->third_.as_string();
		s += ", \"fourth_point\" : " + this->fourth_.as_string();
		s += "}}";
	return s;
}

void Face3d::to_svg(pugi::xml_node& svg_node) {
	svg_node.append_child(pugi::node_comment).set_value("FACE3D -not yet implemented");
}

} /* namespace dxf */
