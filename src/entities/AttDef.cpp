/*
 * AttDef.cpp
 *
 *  Created on: 19 Apr 2022
 *      Author: hobbes
 */

#include "entities/AttDef.h"
#include <cmath>

namespace dxf {

AttDef::AttDef(const vector<Group> &properties) : TextBase(properties) {
    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 2:
                this->tag_ = g.value;
                break;
            case 3:
                this->prompt_ = g.value;
                break;
            case 70:
                this->flags_ = std::stoi(g.value);
                break;
            case 73:
                this->field_length_ = std::stoi(g.value);
                break;
            case 74:
                this->vertical_justification_ = std::stoi(g.value);
                break;
            case 72:
                this->horizontal_justification_ = std::stoi(g.value);
                break;
            case 50:
                this->rotation_ = std::stod(g.value);
                break;
        }
    }
    calc_bounding_box();
}

void AttDef::calc_bounding_box() {
    double w = this->tag_.length() * this->height_ * 0.6;
    double h = this->height_;
    double rad = rotation_ * (3.14159265358979323846 / 180.0);
    double cos_a = cos(rad);
    double sin_a = sin(rad);

    double x_corners[4] = {0, w * cos_a, w * cos_a - h * sin_a, -h * sin_a};
    double y_corners[4] = {0, w * sin_a, w * sin_a + h * cos_a, h * cos_a};

    double min_x = x_corners[0], max_x = x_corners[0];
    double min_y = y_corners[0], max_y = y_corners[0];

    for (int i = 1; i < 4; ++i) {
        if (x_corners[i] < min_x) min_x = x_corners[i];
        if (x_corners[i] > max_x) max_x = x_corners[i];
        if (y_corners[i] < min_y) min_y = y_corners[i];
        if (y_corners[i] > max_y) max_y = y_corners[i];
    }

    this->bounding_box_.x = this->insertion_point_.x_ + min_x;
    this->bounding_box_.y = this->insertion_point_.y_ + min_y;
    this->bounding_box_.width = max_x - min_x;
    this->bounding_box_.heigth = max_y - min_y;
}

string AttDef::to_json() {
    string s = "{\"ATTDEF\" : {";
    s += TextBase::to_string();
    s += ", \"tag\" : \"" + tag_ + "\"";
    s += ", \"prompt\" : \"" + prompt_ + "\"";
    s += ", \"flags\" : " + std::to_string(flags_);
    s += "}}";
    return s;
}

void AttDef::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("ATTDEF");
	writer.StartObject();
	TextBase::write_to_json_writer(writer);
	writer.Key("tag");               writer.String(this->tag_.c_str());
	writer.Key("prompt");            writer.String(this->prompt_.c_str());
	writer.Key("flags");             writer.Int(this->flags_);
	writer.EndObject();
	writer.EndObject();
}

void AttDef::to_svg(pugi::xml_node& svg_node) {
    pugi::xml_node text = svg_node.append_child("text");
    text.append_attribute("x").set_value(insertion_point_.x_);
    text.append_attribute("y").set_value(insertion_point_.y_);
    text.append_attribute("font-size").set_value(height_);
    text.append_attribute("dominant-baseline").set_value("hanging");
    string color = get_svg_color();
    if (color == "#000000") color = "gray"; // Keep distinct look for AttDef if default
    text.append_attribute("fill").set_value(color.c_str());
    text.append_child(pugi::node_pcdata).set_value(tag_.c_str());

    if (rotation_ != 0.0) {
        string transform = "rotate(" + std::to_string(-rotation_) + " " + std::to_string(insertion_point_.x_) + " " + std::to_string(insertion_point_.y_) + ")";
        text.append_attribute("transform").set_value(transform.c_str());
    }
}

} /* namespace dxf */
