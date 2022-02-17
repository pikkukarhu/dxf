/*
 * Entity.h
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */
#pragma once

#ifndef DOCUMENT_ENTITY_H_
#define DOCUMENT_ENTITY_H_

#include <vector>
#include <string>

#include "file/File.h"

using std::vector;
using std::string;

namespace dxf {

class Entity {
protected:
	struct Point_ {
		double x_;
		double y_;
		double z_;
	};

public:

	Entity(const vector<Group> &properties);
	virtual ~Entity();

	Entity(const Entity &other);
	Entity(Entity &&other);
};

} /* namespace dxf */

#endif /* DOCUMENT_ENTITY_H_ */
