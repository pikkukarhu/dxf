/*
 * Insert.h
 *
 *  Created on: 4 Feb 2024
 *      Author: hobbes
 */

#ifndef ENTITIES_INSERT_H_
#define ENTITIES_INSERT_H_

#include "document/Entity.h"
#include "Attrib.h"

namespace dxf {

class Insert final : public dxf::Entity {
private:
	bool attributes_follow_ = false;
	// Attribute
	std::string block_name;
	Point_ insertion_point_;
	double x_scale_ = 1.0;
	double y_scale_ = 1.0;
	std::vector<Attrib> attributes_;
	/*

66 Variable attributes-follow flag (optional; default = 0); if the value of attributes-follow flag is 1,
a series of attribute entities is expected to follow the insert, terminated by a seqend entity
2 Block name
10 Insertion point (in OCS)
DXF: X value; APP: 3D point
20, 30 DXF: Y and Z values of insertion point (in OCS)
41 X scale factor (optional; default = 1)
42 Y scale factor (optional; default = 1)
	 */
protected:
	virtual void calc_bounding_box();

public:
	Insert(const vector<Group> &properties);
	virtual ~Insert();

	virtual string to_json();
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
	virtual void to_svg(pugi::xml_node& svg_node);
};

/*
 *
 *
protected:
	virtual void calc_bounding_box();

public:
	Line(const vector<Group> &properties);
	virtual ~Line() {};

	virtual string to_json();
	virtual void to_svg(pugi::xml_node& svg_node);
};
 */


} /* namespace dxf */

#endif /* ENTITIES_INSERT_H_ */
