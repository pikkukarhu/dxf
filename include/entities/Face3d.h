/*
 * Face3d.h
 *
 *  Created on: 16 Apr 2022
 *      Author: hobbes
 */
#pragma once

#ifndef ENTITIES_FACE3D_H_
#define ENTITIES_FACE3D_H_

#include <pugixml.hpp>
#include "document/Entity.h"

#define __FIRST_EDGE_INVISIBLE 1
#define __SECOND_EDGE_INVISIBLE 2
#define __THIRD_EDGE_INVISIBLE 4
#define __FOURTH_EDGE_INVISIBLE 8

namespace dxf {

class Face3d final : public Entity {
private:
	Point_ first_;		// Group codes 10, 20, 30
	Point_ second_;		// Group codes 11, 21, 31
	Point_ third_;		// Group codes 12, 22, 32
	Point_ fourth_;		// Group codes 13, 23, 33
	unsigned int invisible_edges_;

protected:
	virtual void calc_bounding_box();

public:
	Face3d(const vector<Group> &properties);
	virtual ~Face3d();

	inline bool* invisibleEdges() {
		static bool r[4];
		r[0] = (invisible_edges_ & __FIRST_EDGE_INVISIBLE) != 0;
		r[1] = (invisible_edges_ & __SECOND_EDGE_INVISIBLE) != 0;
		r[2] = (invisible_edges_ & __THIRD_EDGE_INVISIBLE) != 0;
		r[3] = (invisible_edges_ & __FOURTH_EDGE_INVISIBLE) != 0;
		return r;
	}

	virtual string to_json();
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
	virtual void to_svg(pugi::xml_node& svg_node);
};

} /* namespace dxf */

#endif /* ENTITIES_FACE3D_H_ */
