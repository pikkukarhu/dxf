/*
 * Circle.cpp
 *
 *  Created on: 14 Jun 2026
 *      Author: hobbes
 */

#include "Circle.h"

#include <vector>
#include <string>
#include <cmath>

#include "document/Entity.h"

using std::vector;
using std::string;
using std::to_string;

namespace dxf {

void Circle::calc_bounding_box() {
	bounding_box_.x = center_point_.x_ - radius_;
	bounding_box_.y = center_point_.y_ - radius_;
	bounding_box_.width = radius_ * 2;
	bounding_box_.heigth = radius_ * 2;
}

Circle::Circle(const vector<Group> &properties) :Entity(properties) {
	this->center_point_.x_ = 0.0;
	this->center_point_.y_ = 0.0;
	this->center_point_.z_ = 0.0;
	this->radius_ = 0.0;

	for (unsigned int i = 0; i < properties.size(); ++i) {

		Group g = properties[i];
		switch (g.groupcode) {
			case 10:	// Center point X value
				this->center_point_.x_ = stod(g.value);
				break;
			case 20:	// Center point Y value
				this->center_point_.y_ = stod(g.value);
				break;
			case 30:	// Center point Z value
				this->center_point_.z_ = stod(g.value);
				break;
			case 40:	// Radius
				this->radius_ = stod(g.value);
				break;
			case 39:	// Thickness (optional; default = 0)
				this->thicness_ = stod(g.value);
				break;
			case 210: // Extrusion direction X (optional; default = 0, 0, 1)
				this->extrDir_.x_ = stod(g.value);
				break;
			case 220: // Extrusion direction Y
				this->extrDir_.y_ = stod(g.value);
				break;
			case 230: // Extrusion direction Z
				this->extrDir_.z_ = stod(g.value);
				break;
			default:
				break;
		}
	}

	calc_bounding_box();
}

Circle::~Circle() {
}

string Circle::to_json() {
	string s;
	s += "{\"circle\" : {";
	s += to_string();
	s += ", \"center\" : " + center_point_.as_string();
	s += ", \"radius\" : " + to_string();
	s += "}";

	return s;
}

void Circle::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("circle");
	writer.StartObject();
	Entity::write_to_json_writer(writer);
	writer.Key("center"); this->center_point_.write_to_json_writer(writer);
	writer.Key("radius"); writer.Double(this->radius_);
	writer.EndObject();
	writer.EndObject();
}

void Circle::to_svg(pugi::xml_node& svgNode) {
	pugi::xml_node circle = svgNode.append_child("circle");

	// Add original dxf-entity as comment
	char comment[200];
	sprintf(comment, "circle center (%f, %f) radius %f", center_point_.x_, center_point_.y_, radius_);
	circle.append_child(pugi::node_comment).set_value(comment);

	circle.append_attribute("cx").set_value(center_point_.x_);
	circle.append_attribute("cy").set_value(center_point_.y_);
	circle.append_attribute("r").set_value(radius_);
	circle.append_attribute("stroke").set_value("black");
	circle.append_attribute("fill").set_value("none");

	   // Add bounding box
    auto rect = svgNode.append_child("rect");
    rect.append_attribute("x").set_value(bounding_box_.x);
    rect.append_attribute("y").set_value(bounding_box_.y);
    rect.append_attribute("width").set_value(bounding_box_.width);;
    rect.append_attribute("height").set_value(bounding_box_.heigth);
    rect.append_attribute("fill").set_value("none");
    rect.append_attribute("stroke").set_value("blue");
}

} /* namespace dxf */
