/*
 * Entities.cpp
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#include "../document/Document.h"

namespace dxf {


Entities::~Entities() {

	for(auto e = std::begin(entities_); e != std::end(entities_); ++e) {
	   delete e;
	}
}

} /* namespace dxf */
