/*
 * Point.h
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */
#pragma once

#ifndef ENTITIES_POINT_H_
#define ENTITIES_POINT_H_

#include <vector>
#include <string>

#include "document/Entity.h"
#include "file/File.h"

using std::vector;
using std::string;

namespace dxf {

class Point final : public Entity {
private:
	Point_ pt_;

	double thicness_ = 0.0;
	Point_ extrDir_ = {0.0, 0.0, 1.0};

public:
	Point(const vector<Group> &properties);
	virtual ~Point() {};

	inline double x() { return this->pt_.x_; }
	inline double y() { return this->pt_.y_; }
	inline double z() { return this->pt_.z_; }

	virtual string toJson();
};


} /* namespace dxf */

#endif /* ENTITIES_POINT_H_ */
