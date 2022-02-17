/*
 * Line.h
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */
#pragma once

#ifndef ENTITIES_TYPES_LINE_H_
#define ENTITIES_TYPES_LINE_H_

#include <vector>
#include <string>

#include "../Entity.h"
#include "Point.h"

using std::vector;
using std::string;

namespace dxf {

class Line final : public Entity {
private:
	Point start_;
	Point end_;

public:
	Line(const vector<string> &properties);
	virtual ~Line() {};
};


inline Line* createLine(const vector<string> &properties) { return new Line(properties); }

} /* namespace dxf */

#endif /* ENTITIES_TYPES_LINE_H_ */
