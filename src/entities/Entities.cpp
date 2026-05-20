/*
 * Entities.cpp
 *
 *  Created on: 5 Apr 2022
 *      Author: hobbes
 */

#include "entities/Entities.h"


#include <fstream>
#include <iostream>

#include <vector>
#include <string>

#include <pugixml.hpp>

#include "document/Entity.h"
#include "document/EntityFactory.h"

using std::cout;
using std::endl;
using std::cerr;
using std::string;

namespace dxf {

Entities::Entities() {
	// TODO Auto-generated constructor stub

}

Entities::~Entities() {
	// TODO Auto-generated destructor stub
	// TODO delete[] entries in entities_ list.
}

void Entities::read(File* f) {

	EntityFactory ef;

	vector<Group> entity;		// Buffer of one entitity lines
    Group g;
    while (f->readGroup(g)) {

    	if (g.groupcode == 0) {

    		if (entity.size() > 0) {
    			Entity* e = ef.create(entity);
    			if (e == nullptr) {
    				cout << entity[0].value << endl;
    			}
    			else {
    				this->entities_.push_back(e);
    			}
    		}

    		entity.clear();
    		if ( g.value == "ENDSEC") {
    			cout << "] {Entities} ENDSEC " << endl;
    			return;
    		}
    	}

    	entity.push_back(g);
    }
    cerr << "Unexpected end of file" << endl;
}

std::string Entities::to_json() {

	std::string jsonResult = "[";
	for (size_t i = 0; i < entities_.size(); ++i) {
		jsonResult += entities_[i]->to_json();

		// Add a comma if it's not the last element
		if (i < entities_.size() - 1) {
			jsonResult += ", ";
		}
		jsonResult += '\n';
	}
	jsonResult += "]";
	return jsonResult;
}

void Entities::write_json(const std::string& file) {
	std::ofstream outputFile(file);

	// Check if the file is successfully opened
	if (!outputFile.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
	    return;
	}
	outputFile << to_json() << std::endl;
	outputFile.close();
}

void Entities::to_svg(pugi::xml_node& svg_node) {

	for (size_t i = 0; i < entities_.size(); ++i) {
		entities_[i]->to_svg(svg_node);
	}
}

} /* namespace dxf */
