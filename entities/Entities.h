/*
 * Entities.h
 *
 *  Created on: 5 Apr 2022
 *      Author: hobbes
 */

#pragma once

#ifndef ENTITIES_ENTITIES_H_
#define ENTITIES_ENTITIES_H_

#include "document/Section.h"

#include <vector>
#include "document/Entity.h"

namespace dxf {

class Entities final : public Section {
private:
	vector<Entity* > entities_;

public:
	Entities();
	virtual ~Entities();

	virtual void read(File* f);
};

} /* namespace dxf */

#endif /* ENTITIES_ENTITIES_H_ */
