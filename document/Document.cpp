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
#include "tables/Table.h"

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

    Table* currentTable = nullptr;

    Group g;
    while (f.readGroup(g)) {

    	// Start new section    	    continue;
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

    		if (g.groupcode == 0) {

    			if (g.value == "TABLE") {
    				tableHeader = true;
    			}
    			else if (g.value == "ENDTAB") { // table ready

    				if (currentTable != nullptr) {
    					currentTable->createEntry(table);	// Add last entry to table
    				}
    				table.clear();							// Empty used buffer
    			    currentTable = nullptr;					// Current done.
    			    tableHeader = false;
    			}
    			else {

    				if (tableHeader) {

    					currentTable = new Table(table);
    				    this->tables_.push_back(currentTable);
    				    cout << currentTable->toJson() << endl;
    				}
    				else {

    					if (currentTable != nullptr) {
    						// Create entry, Add to currentTable
    						currentTable->createEntry(table);
    					}
    					table.clear();
    					table.push_back(g);
    				}
    				tableHeader = false;
    			}
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

	for (unsigned int i = 0; i < this->tables_.size(); ++i) {
		Table* t = tables_[i];
		cout << t->toJson() << endl;

		delete t;
	}

	for (unsigned int i = 0; i < entities_.size(); ++i) {
		Entity* e = entities_[i];

		// Debug code during development. Shows entityes have been created
		if (i < 20) {
			cout << e->toJson() << endl;
		}
		delete e;
	}
}


} /* namespace dxf */
