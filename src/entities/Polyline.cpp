/*
 * Polyline.cpp
 *
 *  Created on: May 24, 2026
 *      Author: Gemini CLI
 */

#include "entities/Polyline.h"
#include <cmath>
#include <algorithm>

namespace dxf {

Polyline::Polyline(const std::vector<Group> &properties) : Entity(properties) {
    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 70:
                this->flags_ = std::stoi(g.value);
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

void Polyline::addVertex(const std::vector<Group> &vertex_properties) {
    Vertex v;
    for (const auto &g : vertex_properties) {
        switch (g.groupcode) {
            case 10: v.x = std::stod(g.value); break;
            case 20: v.y = std::stod(g.value); break;
            case 30: v.z = std::stod(g.value); break;
            case 42: v.bulge = std::stod(g.value); break;
        }
    }
    vertices_.push_back(v);
    calc_bounding_box();
}

void Polyline::calc_bounding_box() {
    if (vertices_.empty()) {
        bounding_box_ = {0, 0, 0, 0};
        return;
    }

    double min_x = vertices_[0].x;
    double max_x = vertices_[0].x;
    double min_y = vertices_[0].y;
    double max_y = vertices_[0].y;

    for (const auto& v : vertices_) {
        if (v.x < min_x) min_x = v.x;
        if (v.x > max_x) max_x = v.x;
        if (v.y < min_y) min_y = v.y;
        if (v.y > max_y) max_y = v.y;
    }

    bounding_box_.x = min_x;
    bounding_box_.y = min_y;
    bounding_box_.width = max_x - min_x;
    bounding_box_.heigth = max_y - min_y;
}

std::string Polyline::to_json() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    write_to_json_writer(writer);
    return buffer.GetString();
}

void Polyline::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
    writer.StartObject();
    writer.Key("POLYLINE");
    writer.StartObject();
    Entity::write_to_json_writer(writer);
    writer.Key("flags"); writer.Int(flags_);
    
    writer.Key("vertices");
    writer.StartArray();
    for (const auto& v : vertices_) {
        writer.StartObject();
        writer.Key("x"); writer.Double(v.x);
        writer.Key("y"); writer.Double(v.y);
        writer.Key("z"); writer.Double(v.z);
        writer.Key("bulge"); writer.Double(v.bulge);
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
    writer.EndObject();
}

void Polyline::to_svg(pugi::xml_node& svg_node) {
    if (vertices_.empty()) return;

    pugi::xml_node path = svg_node.append_child("path");
    std::string d = "M " + std::to_string(vertices_[0].x) + " " + std::to_string(vertices_[0].y);
    
    for (size_t i = 1; i < vertices_.size(); ++i) {
        d += " L " + std::to_string(vertices_[i].x) + " " + std::to_string(vertices_[i].y);
    }
    
    if (flags_ & 1) { // Closed
        d += " Z";
    }
    
    path.append_attribute("d").set_value(d.c_str());
    
    add_common_svg_attributes(path);

    draw_bounding_box(svg_node);
}

} /* namespace dxf */
