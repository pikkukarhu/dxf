/*
 * EntityFactory.cpp
 *
 *  Created on: 18 Feb 2022
 *      Author: hobbes
 */

#include "document/EntityFactory.h"

#include <string>
#include <vector>

#include "entities/Line.h"
#include "entities/Point.h"

namespace dxf {


inline Entity* createPoint(const vector<Group> &properties) { return new Point(properties); }
inline Entity* createLine(const vector<Group> &properties) { return new Line(properties); }

//Entity* (*)(const vector<Group> &)>


EntityFactory::EntityFactory() {

	this->factories_["LINE"] = createLine;
	this->factories_["POINT"] = createPoint;
}

Entity* EntityFactory::create(const vector<Group> &properties) {

	/*
	 * Get type of entity. FIrst group code 0 is name of entity type
	 */
	string type;
	for (unsigned int i = 0; i < properties.size(); ++i) {
		Group g = properties[i];

		if (g.groupcode == 0) {
			type = g.value;
			break;
		}
	}

	Entity* (*f)(const vector<Group> &) = this->factories_[type];
	return f == nullptr ? nullptr : f(properties);
}

} /* namespace dxf */
