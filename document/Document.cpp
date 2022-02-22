/*
 * Entities.cpp
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#include "Document.h"

#include <fstream>
#include <iostream>

#include "EntityFactory.h"
#include "file/File.h"

using std::cout;
using std::endl;
using std::cerr;


int main(int argc, char** argv) {

	if (argc < 2) {
		std::cerr << "Usage: dxfile file_name" << endl;
	}
	dxf::Document d(argv[1]);
}

namespace dxf {


Document::Document(string file) {
	EntityFactory ef;
	File f(file);

    bool sectionStart = false;
    string section;
    vector<Group> entity;

    Group g;
    while (f.readGroup(g)) {

    	// Start new section
    	if (g.groupcode == 0 && g.value == "SECTION") {
    		sectionStart = true;
    		section = "";
    		continue;
    	}

    	if (g.groupcode == 0 && g.value == "ENDSEC") {
    		section = "";
    		cout << "] ENDSEC" << endl;
    	    continue;
    	}

    	if (g.groupcode == 2 && sectionStart) {
    		sectionStart = false;
    		cout << g.value << " [" << endl;
    		section = g.value;
    		continue;
    	}

    	// Read entities, later for other sections also
    	if (section == "ENTITIES") {

    		if (g.groupcode == 0) { // start new entity

    			// if previous entity > 0, print entity
    			if (entity.size() > 0) {
    			/*	bool first = true;
    				cout << "  {";
    				for (const auto &a : entity) {
    					cout << (first ? "" : ",") << "\"" << a.groupcode << "\" : \"" << a.value << "\"";
    					first = false;
    				}
    				cout << "}" << endl; */

    				Entity* e = ef.create(entity);
    				if (e == nullptr) {
    					cout << entity[0].value << endl;
    				}
    				else {
    					this->entities_.push_back(e);
    				}
    			}
    			entity.clear();
    		}

    		entity.push_back(g);
    		continue;
    	}

    }
}

Document::~Document() {


	for (unsigned int i = 0; i < entities_.size(); ++i) {
		Entity* e = entities_[i];

		cout << e->toString() << endl;

		delete e;
	}
}

void Document::createEntity(const vector<string>& props) {


}


} /* namespace dxf */
