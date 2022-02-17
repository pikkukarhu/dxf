/*
 * Entities.cpp
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#include "Entities.h"

namespace dxf {


Entities::~Entities() {

	for(auto e = std::begin(entities_); e != std::end(entities_); ++e) {
	   delete e;
	}
}

} /* namespace dxf */
