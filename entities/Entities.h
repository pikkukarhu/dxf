/*
 * Entities.h
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef ENTITIES_ENTITIES_H_
#define ENTITIES_ENTITIES_H_

#include <vector>
#include <string>

#include "Entity.h"

using std::vector;
using std::string;

namespace dxf {

class Entities final {
private:
	vector<Entity* > entities_;

public:
	Entities() {}
	virtual ~Entities();

	void createEntity(const vector<string>& props);
};

} /* namespace dxf */

#endif /* ENTITIES_ENTITIES_H_ */
