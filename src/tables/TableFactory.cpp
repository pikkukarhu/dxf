/*
 * TableFactory.cpp
 *
 *  Created on: 10 Mar 2022
 *      Author: hobbes
 */

#include "tables/TableFactory.h"

#include <vector>
#include "entries/Layer.h"
#include "entries/LType.h"
#include "entries/Style.h"
#include "file/File.h"

using std::vector;

namespace dxf {

inline TableEntry* createLayer(const vector<Group> &properties) { return new Layer(properties); }
inline TableEntry* createLType(const vector<Group> &properties) { return new LType(properties); }
inline TableEntry* createStyle(const vector<Group> &properties) { return new Style(properties); }


TableFactory::TableFactory() {
	this->factories_["LAYER"] = createLayer;
	this->factories_["LTYPE"] = createLType;
	this->factories_["STYLE"] = createStyle;

}

TableFactory::~TableFactory() {
	// TODO Auto-generated destructor stub
}

TableEntry* TableFactory::create(const vector<Group> &properties) {
	string type;
	for (unsigned int i = 0; i < properties.size(); ++i) {
		Group g = properties[i];

		if (g.groupcode == 0) {
			type = g.value;
			break;
		}
	}

	TableEntry* (*f)(const vector<Group> &) = this->factories_[type];
	return f == nullptr ? nullptr : f(properties);
}

} /* namespace dxf */
