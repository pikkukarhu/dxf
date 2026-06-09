/*
 * MText.cpp
 *
 *  Created on: 21 Apr 2022
 *      Author: hobbes
 */

#include "entities/MText.h"
#include <cmath>
#include <string>

namespace dxf {

MText::MText(const vector<Group> &properties) : TextBase(properties) {
    this->attachment_point_ = AttachmentPoints::TopLeft;
    this->drawing_direction_ = DrawingDirection::LeftToRight;
    this->mtext_line_spacing_style_ = MtextLineSpacingStyle::AtLeast;
    this->mtext_line_spacing_factor_ = 1.0;
    this->background_fill_setting_ = BackgroundFillSetting::BackgroundFillOff;

    string additional_text = "";
    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 3:
                additional_text += g.value;
                break;
            case 1:
                this->value_ = additional_text + g.value;
                break;
            case 41:
                this->reference_rectangle_width_ = std::stod(g.value);
                break;
            case 71:
                this->attachment_point_ = static_cast<AttachmentPoints>(std::stoi(g.value));
                break;
            case 72:
                this->drawing_direction_ = static_cast<DrawingDirection>(std::stoi(g.value));
                break;
            case 11:
                this->x_axis_direction_vector_.x_ = std::stod(g.value);
                break;
            case 21:
                this->x_axis_direction_vector_.y_ = std::stod(g.value);
                break;
            case 31:
                this->x_axis_direction_vector_.z_ = std::stod(g.value);
                break;
            case 43:
                this->vertical_height_of_mtext_entity_ = std::stod(g.value);
                break;
            case 44:
                this->mtext_line_spacing_factor_ = std::stod(g.value);
                break;
            case 73:
                this->mtext_line_spacing_style_ = static_cast<MtextLineSpacingStyle>(std::stoi(g.value));
                break;
            case 90:
                this->background_fill_setting_ = static_cast<BackgroundFillSetting>(std::stoi(g.value));
                break;
        }
    }
    calc_bounding_box();
}

void MText::calc_bounding_box() {
    this->bounding_box_.x = this->insertion_point_.x_;
    this->bounding_box_.y = this->insertion_point_.y_;
    this->bounding_box_.width = reference_rectangle_width_ > 0 ? reference_rectangle_width_ : (this->value_.length() * this->height_ * 0.5);
    this->bounding_box_.heigth = this->height_ * 1.2;
}

string MText::to_json() {
    string s = "{\"MTEXT\" : {";
    s += TextBase::to_string();
    s += ", \"attachment_point\" : " + std::to_string(static_cast<int>(attachment_point_));
    s += "}}";
    return s;
}

void MText::to_svg(pugi::xml_node& svg_node) {
    pugi::xml_node text = svg_node.append_child("text");
    text.append_attribute("x").set_value(insertion_point_.x_);
    text.append_attribute("y").set_value(insertion_point_.y_);
    text.append_attribute("font-size").set_value(height_);
    text.append_child(pugi::node_pcdata).set_value(value_.c_str());
}

} /* namespace dxf */
