/*
 * EntityFactory.h
 *
 *  Created on: 18 Feb 2022
 *      Author: hobbes
 */
#pragma once

#ifndef DOCUMENT_ENTITYFACTORY_H_
#define DOCUMENT_ENTITYFACTORY_H_

#include <map>
#include <string>

#include "Entity.h"
#include "file/File.h"

using std::map;
using std::string;

namespace dxf {

class EntityFactory final {
private:

	map<string, Entity* (*)(const vector<Group> &)> factories_; /*= {
		{"LINE", createLine},
		{"POINT", createPoint}
	};*/
public:

	EntityFactory();
	virtual ~EntityFactory() {};

	Entity* create(const vector<Group> &properties);
};

} /* namespace dxf */

#endif /* DOCUMENT_ENTITYFACTORY_H_ */
