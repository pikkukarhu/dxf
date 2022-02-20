/*
 * Line.h
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */
#pragma once

#ifndef ENTITIES_LINE_H_
#define ENTITIES_LINE_H_

#include <vector>
#include <string>

#include "document/Entity.h"
#include "file/File.h"

#include "Point.h"

using std::vector;
using std::string;

namespace dxf {

class Line final : public Entity {
private:
	Point_ start_;
	Point_ end_;

	double thicness_ = 0.0;
	Point_ extrDir_ = {0.0, 0.0, 1.0};

public:
	Line(const vector<Group> &properties);
	virtual ~Line() {};
};



} /* namespace dxf */

#endif /* ENTITIES_LINE_H_ */
