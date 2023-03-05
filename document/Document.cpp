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
	File f(file);

    bool sectionStart = false;

    string section;

    vector<Group> entity;		// Buffer of one entitity lines
    vector<Group> table;

    Group g;
    while (f.readGroup(g)) {

    	if (g.groupcode == 0 && g.value == "SECTION") {
    		sectionStart = true;
    		section = "";
    		continue;
    	}
    	if (g.groupcode == 2 && sectionStart) {
    		sectionStart = false;
    		cout << g.value << " [" << endl;

    		section = g.value;
    		if (section == "TABLES") {
    			this->tables_.read(&f);
    		}
    		else if (section == "ENTITIES") {
    			this->entities_.read(&f);
    		}
    		else if (section == "BLOCKS") {
    			this->blocks_.read(&f);
    		}
    		else {	// Default read next section to dev/null
    			Section sec;
    			sec.read(&f);
    		}
    	}

    }
}

Document::~Document() {
}


} /* namespace dxf */
