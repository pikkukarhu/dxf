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
	for (Entity* e : this->entities_) {
		delete e;
	}
	this->entities_.clear();
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
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	write_to_json_writer(writer);
	return buffer.GetString();
}

void Entities::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartArray();
	for (size_t i = 0; i < entities_.size(); ++i) {
		if (entities_[i] != nullptr) {
			entities_[i]->write_to_json_writer(writer);
		}
	}
	writer.EndArray();
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
		if (entities_[i] != nullptr) {
			entities_[i]->to_svg(svg_node);
		}
	}
}

} /* namespace dxf */
