/*
 * Section.cpp
 *
 *  Created on: 5 Apr 2022
 *      Author: hobbes
 */

#include "document/Section.h"

#include <fstream>
#include <iostream>

using std::cout;
using std::endl;
using std::cerr;

namespace dxf {

//Section::Section() {
//	// TODO Auto-generated constructor stub
//
//}
//
//Section::~Section() {
//	// TODO Auto-generated destructor stub
//}

void Section::read(File *f) {

    Group g;
    while (f->readGroup(g)) {
    	if (g.groupcode == 0 && g.value == "ENDSEC") {

    		cout << "] ENDSEC" << endl;
    		return;
    	}
    }
    cerr << "Unexpected end of file" << endl;
}

} /* namespace dxf */
