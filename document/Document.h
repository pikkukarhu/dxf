/*
 * Entities.h
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef DOCUMENT_DOCUMENT_H_
#define DOCUMENT_DOCUMENT_H_

#include <vector>
#include <string>

#include "../document/Entity.h"

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

#endif /* DOCUMENT_DOCUMENT_H_ */
