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
    bool tableHeader = false;

    string section;

    vector<Group> entity;		// Buffer of one entitity lines
    vector<Group> table;

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

    	/*
    	 * Tables section, read known table types. Layer, Ltype, style first
    	 */
    	if (section == "TABLES") {


    		if (g.groupcode == 0 && g.value == "TABLE") {
    			// start new table. Put current table (If exists = not nullptr) to tables of Document (Map, key: table type)
    			// dxf allows many table sections with same type?? need merge if exists
    			tableHeader = true;
    		}
    		else if (g.groupcode == 0 && g.value == "ENDTAB") { // table ready
    			// create (last) table entry from tables-vector and add to current table object.
    			// Close table.

    		}
    		else if (g.groupcode == 2) {	// Start new table entry
    			if (tableHeader) {
    				tableHeader = false;
    				// Finalize table header here. Means create table object from table-vector
    				// Can but table-object all ready here to tables map. Better ???
    				// Better here. Otherwise need put both TABLE & ENDTABLE elements, so that also last table of tables section get saved
    			}
    			else {
    				if (table.size() > 0) {
    					// Create new table entry from table-vector and add to current table object.
    					// Instead flag can use directly table = null.
    				}
    			}
    			table.clear();		// Clean previous objects data
    			table.push_back(g); // Type of table entry.
    		}
    		else {
    			table.push_back(g);	// Add to data vector, used to create new object (Table or table entry) when next start or table ends.
    		}
    	}


    	/*
    	 * In entities section, read known entities, list unknown to stdout
    	 */
    	if (section == "ENTITIES") {

    		if (g.groupcode == 0) { // start new entity

    			// if previous entity > 0, print entity
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
    		}

    		entity.push_back(g);
    		continue;
    	}

    }
}

Document::~Document() {


	for (unsigned int i = 0; i < entities_.size(); ++i) {
		Entity* e = entities_[i];

		// Debug code during development. Shows entityes have been created
		cout << e->toString() << endl;

		delete e;
	}
}


} /* namespace dxf */
