/*
 * Insert.cpp
 *
 *  Created on: 4 Feb 2024
 *      Author: hobbes
 */

#include "entities/Insert.h"

namespace dxf {

Insert::Insert(const vector<Group> &properties) : Entity(properties) {
    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 66:
                this->attributes_follow_ = (g.value == "1");
                break;
            case 2:
                this->block_name = g.value;
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
            case 41:
                this->x_scale_ = std::stod(g.value);
                break;
            case 42:
                this->y_scale_ = std::stod(g.value);
                break;
        }
    }
    calc_bounding_box();
}

void Insert::calc_bounding_box() {
    this->bounding_box_.x = this->insertion_point_.x_;
    this->bounding_box_.y = this->insertion_point_.y_;
    this->bounding_box_.width = 1.0;
    this->bounding_box_.heigth = 1.0;
}

std::string Insert::to_json() {
    string s = "{\"INSERT\" : {";
    s += Entity::to_string();
    s += ", \"block_name\" : \"" + block_name + "\"";
    s += ", \"insertion_point\" : " + insertion_point_.as_string();
    s += ", \"x_scale\" : " + std::to_string(x_scale_);
    s += ", \"y_scale\" : " + std::to_string(y_scale_);
    s += "}}";
    return s;
}

void Insert::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("INSERT");
	writer.StartObject();
	Entity::write_to_json_writer(writer);
	writer.Key("block_name");       writer.String(this->block_name.c_str());
	writer.Key("insertion_point");  this->insertion_point_.write_to_json_writer(writer);
	writer.Key("x_scale");          writer.Double(this->x_scale_);
	writer.Key("y_scale");          writer.Double(this->y_scale_);
	writer.EndObject();
	writer.EndObject();
}

void Insert::to_svg(pugi::xml_node& svg_node) {
    pugi::xml_node use = svg_node.append_child("use");
    string href = "#" + block_name;
    use.append_attribute("href").set_value(href.c_str());
    use.append_attribute("x").set_value(insertion_point_.x_);
    use.append_attribute("y").set_value(insertion_point_.y_);
    
    add_common_svg_attributes(use);
    
    if (x_scale_ != 1.0 || y_scale_ != 1.0) {
        string transform = "scale(" + std::to_string(x_scale_) + " " + std::to_string(y_scale_) + ")";
        use.append_attribute("transform").set_value(transform.c_str());
    }
}

Insert::~Insert() {}

} /* namespace dxf */
