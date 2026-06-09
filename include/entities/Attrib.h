#pragma once
#ifndef __ATTRIB_H__
#define __ATTRIB_H__

#include "entities/TextBase.h"

namespace dxf {

class Attrib final : public TextBase {
private:
	string tag_;
	int flags_ = 0;
	int horizontal_justification_ = 0;
	int vertical_justification_ = 0;

protected:
	virtual void calc_bounding_box() override;

public:
	Attrib(const vector<Group> &properties);
	virtual ~Attrib() {}

	virtual string to_json() override;
	virtual void to_svg(pugi::xml_node& svg_node) override;
};

} /* namespace dxf */

#endif // __ATTRIB_H__
