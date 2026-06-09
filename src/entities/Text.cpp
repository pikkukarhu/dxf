/*
 * Text.cpp
 *
 *  Created on: 3 Dec 2024
 *      Author: hobbes
 */

#include "entities/Text.h"
#include <cmath>

namespace dxf {

Text::Text(const vector<Group> &properties) : TextBase(properties) {
    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 39:
                this->thickness_ = std::stod(g.value);
                break;
            case 11:
                this->second_alignment_point_.x_ = std::stod(g.value);
                break;
            case 21:
                this->second_alignment_point_.y_ = std::stod(g.value);
                break;
            case 31:
                this->second_alignment_point_.z_ = std::stod(g.value);
                break;
            case 72:
                this->horizontal_justification_ = std::stoi(g.value);
                break;
            case 73:
                this->vertical_justification_ = std::stoi(g.value);
                break;
            case 71:
                this->text_generation_flags_ = std::stoi(g.value);
                break;
            case 41:
                this->x_scale_factor_ = std::stod(g.value);
                break;
            case 51:
                this->oblique_angle_ = std::stod(g.value);
                break;
        }
    }
    calc_bounding_box();
}

void Text::calc_bounding_box() {
    this->bounding_box_.x = this->insertion_point_.x_;
    this->bounding_box_.y = this->insertion_point_.y_;
    this->bounding_box_.width = this->value_.length() * this->height_ * 0.6;
    this->bounding_box_.heigth = this->height_;
}

string Text::to_json() {
    string s = "{\"TEXT\" : {";
    s += TextBase::to_string();
    s += ", \"thickness\" : " + std::to_string(thickness_);
    s += ", \"horizontal_justification\" : " + std::to_string(horizontal_justification_);
    s += ", \"vertical_justification\" : " + std::to_string(vertical_justification_);
    s += "}}";
    return s;
}

void Text::to_svg(pugi::xml_node& svg_node) {
    pugi::xml_node text = svg_node.append_child("text");
    text.append_attribute("x").set_value(insertion_point_.x_);
    text.append_attribute("y").set_value(insertion_point_.y_);
    text.append_attribute("font-size").set_value(height_);
    text.append_child(pugi::node_pcdata).set_value(value_.c_str());
    
    if (rotation_ != 0.0) {
        string transform = "rotate(" + std::to_string(-rotation_) + " " + std::to_string(insertion_point_.x_) + " " + std::to_string(insertion_point_.y_) + ")";
        text.append_attribute("transform").set_value(transform.c_str());
    }
}

} /* namespace dxf */
