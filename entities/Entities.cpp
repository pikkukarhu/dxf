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

#include "document/Entity.h"
#include "document/EntityFactory.h"

using std::cout;
using std::endl;
using std::cerr;

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
//
//		if (g.groupcode == 0) { // start new entity
//
//			if (entity.size() > 0) {
//
//				Entity* e = ef.create(entity);
//				if (e == nullptr) {
//					cout << entity[0].value << endl;
//				}
//				else {
//					this->entities_.push_back(e);
//				}
//			}
//			entity.clear();
//		}


    }
    cerr << "Unexpected end of file" << endl;
}

} /* namespace dxf */
