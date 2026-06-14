/*
 * Circle.h
 *
 *  Created on: 14 Jun 2026
 *      Author: hobbes
 */

#ifndef ENTITIES_CIRCLE_H_
#define ENTITIES_CIRCLE_H_

#include <vector>
#include <string>
#include <pugixml.hpp>

#include "document/Entity.h"

namespace dxf {

using std::vector;
using std::string;

class Circle final : public Entity {
private:
	Point_ center_point_;	// 10, 20, 30
	double radius_;			// 40

	double thicness_ = 0.0;	// 39
	Point_ extrDir_ = {0.0, 0.0, 1.0};	// 210, 220, 230

protected:
	virtual void calc_bounding_box();

public:
	Circle(const vector<Group> &properties);
	virtual ~Circle();

	virtual string to_json();
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
	virtual void to_svg(pugi::xml_node& svgNode);
};

} /* namespace dxf */

#endif /* ENTITIES_CIRCLE_H_ */
