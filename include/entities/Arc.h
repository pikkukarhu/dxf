/*
 * Arc.h
 *
 *  Created on: 16 Apr 2022
 *      Author: hobbes
 */

#ifndef ENTITIES_ARC_H_
#define ENTITIES_ARC_H_

#include <vector>
#include <string>
#include <pugixml.hpp>

#include "document/Entity.h"

namespace dxf {

using std::vector;
using std::string;

class Arc final : public Entity {
private:
	Point_ center_point_;	// 10, 20, 30
	double radius_;			// 40
	double start_angle_;	// 50
	double end_angle_;		// 51

	double thicness_ = 0.0;	// 39
	Point_ extrDir_ = {0.0, 0.0, 1.0};	// 210, 220, 230

protected:
	virtual void calc_bounding_box();

public:
	Arc(const vector<Group> &properties);
	virtual ~Arc();

	virtual string to_json();
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
	virtual void to_svg(pugi::xml_node& svgNode);
};

} /* namespace dxf */

#endif /* ENTITIES_ARC_H_ */
