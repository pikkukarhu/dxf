/*
 * Eclipse.h
 *
 *  Created on: 6 Jan 2024
 *      Author: hobbes
 */

#ifndef ENTITIES_ELLIPSE_H_
#define ENTITIES_ELLIPSE_H_

#include <pugixml.hpp>
#include "document/Entity.h"

namespace dxf {

class Ellipse final : public Entity {
private:
	//	100 : Subclass marker (AcDbEllipse)
	//	10 : Center point (in WCS)
	//	   DXF: X value; APP: 3D point
	//	20, 30 : DXF: Y and Z values of center point (in WCS)
	//	11 : Endpoint of major axis, relative to the center (in WCS)
	//	   DXF: X value; APP: 3D point
	//	21, 31 : DXF: Y and Z values of endpoint of major axis, relative to the center (in WCS)
	//	210 : Extrusion direction (optional; default = 0, 0, 1)
	//	    DXF: X value; APP: 3D vector
	//	220, 230 : DXF: Y and Z values of extrusion direction (optional)
	//	40 : Ratio of minor axis to major axis
	//	41 : Start parameter (this value is 0.0 for a full ellipse)
	//	42 : End parameter (this value is 2pi for a full ellipse)

	Point_ center_point_; 	// 10, 20, 30
	Point_ end_point_;		// 11, 21, 31
	Point_ extrusion_dir_;	// 210, 220, 230
	double ratio_;			// 40
	double start_angle_;	// 41
	double end_angle_;		// 42

protected:
	virtual void calc_bounding_box();

public:
	Ellipse(const vector<Group> &properties);
	virtual ~Ellipse() {};

	virtual string to_json();
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
	virtual void to_svg(pugi::xml_node& svg_node);
};

} /* namespace dxf */

#endif /* ENTITIES_ELLIPSE_H_ */
