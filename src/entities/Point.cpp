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

void Point::calc_bounding_box() {
	bounding_box_.x = pt_.x_;
	bounding_box_.y = pt_.y_;
	bounding_box_.width = 0;
	bounding_box_.heigth = 0;
}

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

string Point::to_json() {
	string s;
	s += "{\"point\" : {";
	s += to_string();

	s += ", \"coordinates\" : " + pt_.as_string();
	s += "}";

	return s;
}

void Point::to_svg(pugi::xml_node& svgNode) {
	//  <line x1="0" y1="80" x2="100" y2="20" stroke="black" />
	pugi::xml_node line = svgNode.append_child("line");

    // Add original dxf-entity as comment
    char comment[200];
    sprintf(comment, "point (%f, %f)", pt_.x_, pt_.y_);
    line.append_child(pugi::node_comment).set_value(comment);


    line.append_attribute("x1").set_value(pt_.x_);
    line.append_attribute("y1").set_value(pt_.y_);
    line.append_attribute("x2").set_value(pt_.x_);
    line.append_attribute("y2").set_value(pt_.y_);

    // Set fill and color
    line.append_attribute("fill").set_value("none");
    line.append_attribute("stroke").set_value("black");
}

} /* namespace dxf */
