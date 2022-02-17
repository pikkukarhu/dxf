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

public:
	Point(const vector<Group> &properties);

	virtual ~Point() {};
};

} /* namespace dxf */

#endif /* ENTITIES_POINT_H_ */
