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
#include "entities/Face3d.h"
#include "entities/Arc.h"
#include "entities/Ellipse.h"
#include "entities/Text.h"
#include "entities/MText.h"
#include "entities/Insert.h"
#include "entities/AttDef.h"
#include "entities/Attrib.h"

namespace dxf {


inline Entity* createPoint(const vector<Group> &properties) { return new Point(properties); }
inline Entity* createLine(const vector<Group> &properties) { return new Line(properties); }
inline Entity* createFace3d(const vector<Group> &properties) { return new Face3d(properties); }
inline Entity* createArc(const vector<Group> &properties) { return new Arc(properties); }
inline Entity* createEllipse(const vector<Group> &properties) { return new Ellipse(properties); }
inline Entity* createText(const vector<Group> &properties) { return new Text(properties); }
inline Entity* createMText(const vector<Group> &properties) { return new MText(properties); }
inline Entity* createInsert(const vector<Group> &properties) { return new Insert(properties); }
inline Entity* createAttDef(const vector<Group> &properties) { return new AttDef(properties); }
inline Entity* createAttrib(const vector<Group> &properties) { return new Attrib(properties); }


EntityFactory::EntityFactory() {

	this->factories_["LINE"] = createLine;
	this->factories_["POINT"] = createPoint;
	this->factories_["3DFACE"] = createFace3d;
	this->factories_["ARC"] = createArc;
	this->factories_["ELLIPSE"] = createEllipse;
	this->factories_["TEXT"] = createText;
	this->factories_["MTEXT"] = createMText;
	this->factories_["INSERT"] = createInsert;
	this->factories_["ATTDEF"] = createAttDef;
	this->factories_["ATTRIB"] = createAttrib;
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
