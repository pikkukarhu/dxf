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
	// calculate bouding box;
	calc_bounding_box();
}

void Line::calc_bounding_box() {
	bounding_box_.x = start_.x_ < end_.x_ ? start_.x_ : end_.x_;
	bounding_box_.y = start_.y_ >= end_.y_ ? end_.y_ : start_.y_;
	bounding_box_.width = std::abs(end_.x_ - start_.x_);
	bounding_box_.heigth = std::abs(end_.y_ - start_.y_);
}

string Line::to_json() {
	string s;
		s += "{\"line\" : {";
		s += to_string();
		s += ", \"start_point\" : " + this->start_.as_string();
		s += ", \"end_point\" : " + this->end_.as_string();
		s += "}}";
	return s;
}

void Line::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("line");
	writer.StartObject();
	Entity::write_to_json_writer(writer);
	writer.Key("start_point"); this->start_.write_to_json_writer(writer);
	writer.Key("end_point"); this->end_.write_to_json_writer(writer);
	writer.EndObject();
	writer.EndObject();
}

void Line::to_svg(pugi::xml_node& svg_node) {
	//  <line x1="0" y1="80" x2="100" y2="20" stroke="black" />
	pugi::xml_node line = svg_node.append_child("line");

    line.append_attribute("x1").set_value(start_.x_);
    line.append_attribute("y1").set_value(start_.y_);
    line.append_attribute("x2").set_value(end_.x_);
    line.append_attribute("y2").set_value(end_.y_);

    // Set fill and color
    line.append_attribute("fill").set_value("none");
    line.append_attribute("stroke").set_value(get_svg_color().c_str());

    draw_bounding_box(svg_node);
}

} /* namespace dxf */
