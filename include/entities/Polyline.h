/*
 * Polyline.h
 *
 *  Created on: May 24, 2026
 *      Author: Gemini CLI
 */

#ifndef ENTITIES_POLYLINE_H_
#define ENTITIES_POLYLINE_H_

#include <vector>
#include <string>
#include <pugixml.hpp>
#include "document/Entity.h"

namespace dxf {

class Polyline final : public Entity {
private:
    struct Vertex {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        double bulge = 0.0;
    };
    std::vector<Vertex> vertices_;
    int flags_ = 0; // 70
    Point_ extrusion_direction_ = {0.0, 0.0, 1.0};

protected:
    virtual void calc_bounding_box() override;

public:
    Polyline(const std::vector<Group> &properties);
    virtual ~Polyline() {}

    void addVertex(const std::vector<Group> &vertex_properties);

    virtual std::string to_json() override;
    virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
    virtual void to_svg(pugi::xml_node& svg_node) override;
};

} /* namespace dxf */

#endif /* ENTITIES_POLYLINE_H_ */
