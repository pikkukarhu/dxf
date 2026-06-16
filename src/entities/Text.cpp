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
            case 50:
                this->rotation_ = std::stod(g.value);
                break;
            case 51:
                this->oblique_angle_ = std::stod(g.value);
                break;
        }
    }
    calc_bounding_box();
}

void Text::calc_bounding_box() {
    double w = this->value_.length() * this->height_ * 0.6; // Rough estimate
    double h = this->height_;
    double rad = rotation_ * (3.14159265358979323846 / 180.0);
    double cos_a = cos(rad);
    double sin_a = sin(rad);

    // Corners relative to insertion point (assuming hanging baseline for calculation)
    // DXF CCW rotation: x' = x*cos - y*sin, y' = x*sin + y*cos
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

string Text::to_json() {
    string s = "{\"TEXT\" : {";
    s += TextBase::to_string();
    s += ", \"thickness\" : " + std::to_string(thickness_);
    s += ", \"horizontal_justification\" : " + std::to_string(horizontal_justification_);
    s += ", \"vertical_justification\" : " + std::to_string(vertical_justification_);
    s += "}}";
    return s;
}

void Text::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("TEXT");
	writer.StartObject();
	TextBase::write_to_json_writer(writer);
	writer.Key("thickness");                writer.Double(this->thickness_);
	writer.Key("horizontal_justification"); writer.Int(this->horizontal_justification_);
	writer.Key("vertical_justification");   writer.Int(this->vertical_justification_);
	writer.EndObject();
	writer.EndObject();
}

void Text::to_svg(pugi::xml_node& svg_node) {
    pugi::xml_node text = svg_node.append_child("text");
    text.append_attribute("x").set_value(insertion_point_.x_);
    text.append_attribute("y").set_value(insertion_point_.y_);
    text.append_attribute("font-size").set_value(height_);
    text.append_attribute("dominant-baseline").set_value("hanging");
    text.append_attribute("fill").set_value(get_svg_color().c_str());
    text.append_child(pugi::node_pcdata).set_value(value_.c_str());
    
    if (rotation_ != 0.0) {
        string transform = "rotate(" + std::to_string(-rotation_) + " " + std::to_string(insertion_point_.x_) + " " + std::to_string(insertion_point_.y_) + ")";
        text.append_attribute("transform").set_value(transform.c_str());
    }

    draw_bounding_box(svg_node);
}

} /* namespace dxf */
