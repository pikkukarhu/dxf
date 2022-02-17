/*
 * Point.h
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */
#pragma once

#ifndef ENTITIES_TYPES_POINT_H_
#define ENTITIES_TYPES_POINT_H_

#include <vector>
#include <string>

#include "../Entity.h"

using std::vector;
using std::string;

namespace dxf {

class Point final : public Entity {
private:
	double x_;
	double y_;
	double z_;

public:
	Point(const vector<string> properies);
	inline Point(const double x, const double y, const double z) { this->x_= x; this->y_= y; this->z_ = z; }

	virtual ~Point() {};
};

} /* namespace dxf */

#endif /* ENTITIES_TYPES_POINT_H_ */
