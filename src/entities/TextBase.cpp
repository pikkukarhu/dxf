/*
 * TextBase.cpp
 *
 *  Created on: May 23, 2026
 *      Author: Gemini CLI
 */

#include "entities/TextBase.h"

namespace dxf {

TextBase::TextBase(const vector<Group> &properties) : Entity(properties) {
    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 1:
                this->value_ = g.value;
                break;
            case 7:
                this->style_name_ = g.value;
                break;
            case 10:
                this->insertion_point_.x_ = std::stod(g.value);
                break;
            case 20:
                this->insertion_point_.y_ = std::stod(g.value);
                break;
            case 30:
                this->insertion_point_.z_ = std::stod(g.value);
                break;
            case 40:
                this->height_ = std::stod(g.value);
                break;
            case 50:
                this->rotation_ = std::stod(g.value);
                break;
            case 210:
                this->extrusion_direction_.x_ = std::stod(g.value);
                break;
            case 220:
                this->extrusion_direction_.y_ = std::stod(g.value);
                break;
            case 230:
                this->extrusion_direction_.z_ = std::stod(g.value);
                break;
        }
    }
}

string TextBase::to_string() {
    string s = Entity::to_string();
    s += ", \"value\" : \"" + value_ + "\"";
    s += ", \"style_name\" : \"" + style_name_ + "\"";
    s += ", \"insertion_point\" : " + insertion_point_.as_string();
    s += ", \"height\" : " + std::to_string(height_);
    s += ", \"rotation\" : " + std::to_string(rotation_);
    s += ", \"extrusion_direction\" : " + extrusion_direction_.as_string();
    return s;
}

void TextBase::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	Entity::write_to_json_writer(writer);
	writer.Key("value");               writer.String(this->value_.c_str());
	writer.Key("style_name");          writer.String(this->style_name_.c_str());
	writer.Key("insertion_point");     this->insertion_point_.write_to_json_writer(writer);
	writer.Key("height");              writer.Double(this->height_);
	writer.Key("rotation");            writer.Double(this->rotation_);
	writer.Key("extrusion_direction"); this->extrusion_direction_.write_to_json_writer(writer);
}

} /* namespace dxf */
