/*
 * AttDef.cpp
 *
 *  Created on: 19 Apr 2022
 *      Author: hobbes
 */

#include "entities/AttDef.h"

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
        }
    }
    calc_bounding_box();
}

void AttDef::calc_bounding_box() {
    this->bounding_box_.x = this->insertion_point_.x_;
    this->bounding_box_.y = this->insertion_point_.y_;
    this->bounding_box_.width = this->tag_.length() * this->height_ * 0.6;
    this->bounding_box_.heigth = this->height_;
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

void AttDef::to_svg(pugi::xml_node& svg_node) {
    pugi::xml_node text = svg_node.append_child("text");
    text.append_attribute("x").set_value(insertion_point_.x_);
    text.append_attribute("y").set_value(insertion_point_.y_);
    text.append_attribute("font-size").set_value(height_);
    text.append_attribute("fill").set_value("gray");
    text.append_child(pugi::node_pcdata).set_value(tag_.c_str());
}

} /* namespace dxf */
