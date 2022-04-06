/*
 * Tables.cpp
 *
 *  Created on: 6 Apr 2022
 *      Author: hobbes
 */

#include "tables/Tables.h"

#include <fstream>
#include <iostream>
#include "tables/Table.h"

#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::cerr;

namespace dxf {

Tables::Tables() {
	// TODO Auto-generated constructor stub

}

Tables::~Tables() {
	// TODO Auto-generated destructor stub
}

void Tables::read(File* f) {

	bool tableHeader = false;
    vector<Group> table;
    Table* currentTable = nullptr;

    Group g;
    while (f->readGroup(g)) {

    	if (g.groupcode == 0) {

    		if ( g.value == "ENDSEC") {
    			cout << "] {Tables} ENDSEC " << endl;
    		    return;
    		}

    		if (g.value == "TABLE") {
    			tableHeader = true;
    		}
    		else if (g.value == "ENDTAB") { // table ready

    			if (currentTable != nullptr) {
    				currentTable->createEntry(table);	// Add last entry to table
    			}
    			table.clear();						// Empty used buffer
    			currentTable = nullptr;				// Current done.
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
    cerr << "Unexpected end of file" << endl;
}

} /* namespace dxf */
