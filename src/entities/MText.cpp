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
            case 50:
                // MTEXT group 50 is radians, convert to degrees for consistency in TextBase::rotation_
                this->rotation_ = std::stod(g.value) * (180.0 / 3.14159265358979323846);
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
    double w = reference_rectangle_width_ > 0 ? reference_rectangle_width_ : (this->value_.length() * this->height_ * 0.5);
    double h = this->height_ * 1.2; // Rough estimate for multiline
    double rad = rotation_ * (3.14159265358979323846 / 180.0);
    double cos_a = cos(rad);
    double sin_a = sin(rad);

    // Corners relative to insertion point (assuming hanging baseline for calculation)
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

string MText::to_json() {
    string s = "{\"MTEXT\" : {";
    s += TextBase::to_string();
    s += ", \"attachment_point\" : " + std::to_string(static_cast<int>(attachment_point_));
    s += "}}";
    return s;
}

void MText::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("MTEXT");
	writer.StartObject();
	TextBase::write_to_json_writer(writer);
	writer.Key("attachment_point"); writer.Int(static_cast<int>(this->attachment_point_));
	writer.Key("drawing_direction"); writer.Int(static_cast<int>(this->drawing_direction_));
	writer.Key("max_horizontal_width"); writer.Double(this->reference_rectangle_width_);
	writer.EndObject();
	writer.EndObject();
}

void MText::to_svg(pugi::xml_node& svg_node) {
    pugi::xml_node text = svg_node.append_child("text");
    text.append_attribute("x").set_value(insertion_point_.x_);
    text.append_attribute("y").set_value(insertion_point_.y_);
    text.append_attribute("font-size").set_value(height_);
    text.append_attribute("dominant-baseline").set_value("hanging");
    text.append_attribute("fill").set_value(get_svg_color().c_str());
    
    if (rotation_ != 0.0) {
        string transform = "rotate(" + std::to_string(-rotation_) + " " + std::to_string(insertion_point_.x_) + " " + std::to_string(insertion_point_.y_) + ")";
        text.append_attribute("transform").set_value(transform.c_str());
    }

    // MText can have multiple lines separated by \P or other markers
    // For now, we just put the raw string
    text.append_child(pugi::node_pcdata).set_value(value_.c_str());

    draw_bounding_box(svg_node);
}

} /* namespace dxf */
