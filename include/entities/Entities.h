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
#include <pugixml.hpp>

#include "document/Entity.h"

namespace dxf {

class Entities final : public Section {
private:
	vector<Entity* > entities_;

public:
	Entities();
	virtual ~Entities();

	virtual void read(File* f);
	virtual std::string to_json();
	virtual void write_json(const std::string& file);

	void to_svg(pugi::xml_node& svgNode);
};

} /* namespace dxf */

#endif /* ENTITIES_ENTITIES_H_ */
