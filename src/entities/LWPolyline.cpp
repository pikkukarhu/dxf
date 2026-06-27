/*
 * LWPolyline.cpp
 *
 *  Created on: May 24, 2026
 *      Author: Gemini CLI
 */

#include "entities/LWPolyline.h"
#include <cmath>
#include <algorithm>

namespace dxf {

LWPolyline::LWPolyline(const std::vector<Group> &properties) : Entity(properties) {
    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 70:
                this->flags_ = std::stoi(g.value);
                break;
            case 43:
                this->constant_width_ = std::stod(g.value);
                break;
            case 38:
                this->elevation_ = std::stod(g.value);
                break;
            case 39:
                this->thickness_ = std::stod(g.value);
                break;
            case 10:
                vertices_.push_back(Vertex());
                vertices_.back().x = std::stod(g.value);
                break;
            case 20:
                if (!vertices_.empty()) {
                    vertices_.back().y = std::stod(g.value);
                }
                break;
            case 40:
                if (!vertices_.empty()) {
                    vertices_.back().start_width = std::stod(g.value);
                }
                break;
            case 41:
                if (!vertices_.empty()) {
                    vertices_.back().end_width = std::stod(g.value);
                }
                break;
            case 42:
                if (!vertices_.empty()) {
                    vertices_.back().bulge = std::stod(g.value);
                }
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
    calc_bounding_box();
}

void LWPolyline::calc_bounding_box() {
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

    // TODO: if bulge != 0, the arc might extend beyond the vertex bounds.
    // For now, we use a simple vertex-based bounding box.

    bounding_box_.x = min_x;
    bounding_box_.y = min_y;
    bounding_box_.width = max_x - min_x;
    bounding_box_.heigth = max_y - min_y;
}

std::string LWPolyline::to_json() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    write_to_json_writer(writer);
    return buffer.GetString();
}

void LWPolyline::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
    writer.StartObject();
    writer.Key("LWPOLYLINE");
    writer.StartObject();
    Entity::write_to_json_writer(writer);
    writer.Key("flags");             writer.Int(flags_);
    writer.Key("constant_width");    writer.Double(constant_width_);
    writer.Key("elevation");         writer.Double(elevation_);
    writer.Key("thickness");         writer.Double(thickness_);
    
    writer.Key("vertices");
    writer.StartArray();
    for (const auto& v : vertices_) {
        writer.StartObject();
        writer.Key("x");           writer.Double(v.x);
        writer.Key("y");           writer.Double(v.y);
        writer.Key("bulge");       writer.Double(v.bulge);
        writer.Key("start_width"); writer.Double(v.start_width);
        writer.Key("end_width");   writer.Double(v.end_width);
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
    writer.EndObject();
}

void LWPolyline::to_svg(pugi::xml_node& svg_node) {
    if (vertices_.empty()) return;

    pugi::xml_node path = svg_node.append_child("path");
    std::string d = "M " + std::to_string(vertices_[0].x) + " " + std::to_string(vertices_[0].y);
    
    for (size_t i = 1; i < vertices_.size(); ++i) {
        // Simplified: only drawing straight lines, ignoring bulge for now
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
