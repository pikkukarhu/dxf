/*
 * Entities.cpp
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#include "../document/Document.h"

namespace dxf {


Entities::~Entities() {

	for (int i = 0; i < entities_.size(); ++i) {
		Entity* e = entities_[i];
		delete e;
	}
}

} /* namespace dxf */
