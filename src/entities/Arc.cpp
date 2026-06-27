/*
 * Arc.cpp
 *
 *  Created on: 16 Apr 2022
 *      Author: hobbes
 */

#include <pugixml.hpp>
#include <cmath>

#include "entities/Arc.h"

namespace dxf {

using std::to_string;

void Arc::calc_bounding_box() {
	bounding_box_.x = center_point_.x_ - radius_;
	bounding_box_.y = center_point_.y_ - radius_;
	bounding_box_.width = 2.0 * radius_;
	bounding_box_.heigth = 2.0 * radius_;
}

Arc::Arc(const vector<Group> &properties) :Entity(properties) {

	for (unsigned int i = 0; i < properties.size(); ++i) {

		Group g = properties[i];
		switch (g.groupcode) {
			// 100 Subclass marker (AcDbLine) - read in Entity
			case 39:	// Thickness (optional; default = 0)
				this->thicness_ = stod(g.value);
				break;
			case 10:	// Start point (in WCS)
				this->center_point_.x_ = stod(g.value);
				break;
			case 20:
				this->center_point_.y_ = stod(g.value);
				break;
			case 30:
				this->center_point_.z_ = stod(g.value);
				break;
			case 40:
				this->radius_ = stod(g.value);
				break;
			case 50:
				this->start_angle_ =  stod(g.value);
				break;
			case 51:
				this->end_angle_ =  stod(g.value);
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
	calc_bounding_box();
}

Arc::~Arc() {
	// TODO Auto-generated destructor stub
}

string Arc::to_json() {
	string s;
	s += "{\"arc\" : {";
		s += to_string();
		s += ", \"center_point\" : " + this->center_point_.as_string();
		s += ", \"start_angle\" : "  + std::to_string(this->start_angle_) + ", ";
		s += ", \"end_angle\" : "  + std::to_string(this->end_angle_) + "} ";
		s += ", \"radius\" : " + std::to_string(radius_);
		s += "}}";
	return s;
}

void Arc::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("arc");
	writer.StartObject();
	Entity::write_to_json_writer(writer);
	writer.Key("center_point"); this->center_point_.write_to_json_writer(writer);
	writer.Key("start_angle"); writer.Double(this->start_angle_);
	writer.Key("end_angle"); writer.Double(this->end_angle_);
	writer.Key("radius"); writer.Double(this->radius_);
	writer.EndObject();
	writer.EndObject();
}

void Arc::to_svg(pugi::xml_node& svg_node) {

    pugi::xml_node path = svg_node.append_child("path");

    // Add original dxf-entity as comment
    char comment[200];
    sprintf(comment, "arc. center (%f, %f), start %f, end %f, radius %f", center_point_.x_, center_point_.y_, start_angle_, end_angle_, radius_);
    path.append_child(pugi::node_comment).set_value(comment);

    // Calculate path definition from entity definition
    double start_x = center_point_.x_  + radius_ * cos(start_angle_ * M_PI / 180.0);
    double start_y = center_point_.y_  + radius_ * sin(start_angle_ * M_PI / 180.0);
    double end_x = center_point_.x_  + radius_ * cos(end_angle_ * M_PI / 180.0);
    double end_y = center_point_.y_  + radius_ * sin(end_angle_ * M_PI / 180.0);

    // Append control points to path
    char path_data[100];
    sprintf(path_data, "M%f %f A %f %f, 0, 0, 1, %f %f", start_x, start_y, radius_, radius_, end_x, end_y);
    path.append_attribute("d").set_value(path_data);

    add_common_svg_attributes(path);

    draw_bounding_box(svg_node);
}

} /* namespace dxf */
