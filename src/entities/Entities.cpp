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
#include "entities/Polyline.h"

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
    Polyline* currentPolyline = nullptr;

    while (f->readGroup(g)) {

    	if (g.groupcode == 0) {

    		if (entity.size() > 0) {
                if (currentPolyline != nullptr) {
                    if (entity[0].value == "VERTEX") {
                        currentPolyline->addVertex(entity);
                    }
                    // SEQEND will fall through to be checked below
                } else {
    			    Entity* e = ef.create(entity);
    			    if (e == nullptr) {
    				    cout << "Unknown entity: " << entity[0].value << endl;
    			    }
    			    else {
    				    this->entities_.push_back(e);
                        if (entity[0].value == "POLYLINE") {
                            currentPolyline = dynamic_cast<Polyline*>(e);
                        }
    			    }
                }
    		}

    		entity.clear();
    		if ( g.value == "ENDSEC") {
    			cout << "] {Entities} ENDSEC " << endl;
    			return;
    		}
            if ( g.value == "SEQEND") {
                currentPolyline = nullptr;
            }
    	}

    	entity.push_back(g);
    }
    cerr << "Unexpected end of file" << endl;
}

void Entities::resolve(const Tables& tables, bool isBlackBackground) {
	for (size_t i = 0; i < entities_.size(); ++i) {
		if (entities_[i] != nullptr) {
			entities_[i]->resolve(tables, isBlackBackground);
		}
	}
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

BoundingBox Entities::get_bounding_box() const {
    BoundingBox bb;
    for (Entity* e : entities_) {
        if (e != nullptr) {
            bb.combine(e->get_bounding_box().to_bounding_box());
        }
    }
    return bb;
}

} /* namespace dxf */
