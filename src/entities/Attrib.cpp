/*
 * Attrib.cpp
 *
 *  Created on: 5 Feb 2024
 *      Author: hobbes
 */

#include "entities/Attrib.h"

namespace dxf {

Attrib::Attrib(const vector<Group> &properties) : TextBase(properties) {
    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 2:
                this->tag_ = g.value;
                break;
            case 70:
                this->flags_ = std::stoi(g.value);
                break;
            case 72:
                this->horizontal_justification_ = std::stoi(g.value);
                break;
            case 74:
                this->vertical_justification_ = std::stoi(g.value);
                break;
        }
    }
    calc_bounding_box();
}

void Attrib::calc_bounding_box() {
    this->bounding_box_.x = this->insertion_point_.x_;
    this->bounding_box_.y = this->insertion_point_.y_;
    this->bounding_box_.width = this->value_.length() * this->height_ * 0.6;
    this->bounding_box_.heigth = this->height_;
}

string Attrib::to_json() {
    string s = "{\"ATTRIB\" : {";
    s += TextBase::to_string();
    s += ", \"tag\" : \"" + tag_ + "\"";
    s += ", \"flags\" : " + std::to_string(flags_);
    s += "}}";
    return s;
}

void Attrib::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("ATTRIB");
	writer.StartObject();
	TextBase::write_to_json_writer(writer);
	writer.Key("tag");               writer.String(this->tag_.c_str());
	writer.Key("flags");             writer.Int(this->flags_);
	writer.EndObject();
	writer.EndObject();
}

void Attrib::to_svg(pugi::xml_node& svg_node) {
    pugi::xml_node text = svg_node.append_child("text");
    text.append_attribute("x").set_value(insertion_point_.x_);
    text.append_attribute("y").set_value(insertion_point_.y_);
    text.append_attribute("font-size").set_value(height_);
    text.append_child(pugi::node_pcdata).set_value(value_.c_str());
}

} /* namespace dxf */
