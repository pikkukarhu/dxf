/*
 * Document.cpp
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#include "File.h"

#include <stdlib.h>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::cerr;
using std::vector;

int main(int argc, char** argv) {

	if (argc < 2) {
		cerr << "Usage: dxfile file_name" << endl;
	}
	dxf::Document d(argv[1]);
	d.readFile();
}

namespace dxf {

std::istream& safeGetline(std::istream& is, std::string& t)     {
    t.clear();

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();

        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char) c;
        }
    }
    is.setstate(std::ios::eofbit);
    return is;
}



std::istream& Document::readGroup(std::istream& is, Group& g) {

	string s;
	if (! safeGetline(is, s) || s == "") {
		return is;
	}
	try {
		g.groupcode = stoi(s);
	}
	catch (...) {
		g.groupcode = -15;
		cerr << "Error convert group code [" << s << "] to integer!" << endl;
	}
	safeGetline(is, g.value);
	if (g.value == "EOF") {
		is.setstate(std::ios::eofbit);
	}

	return is;
}

/*
 * Read all lines of document
 */
int Document::readFile() {
    ifstream fs(this->file_);
    string l;

    // Check file exists
    if (!fs.is_open()) {
        return EXIT_FAILURE;
    }
    setlocale(6, "us_ascii");

    bool sectionStart = false;
    string section;
    vector<Group> entity;

    Group g;
    while (readGroup(fs, g)) {

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
    				bool first = true;
    				cout << "  {";
    				for (const auto &a : entity) {
    					cout << (first ? "" : ",") << "\"" << a.groupcode << "\" : \"" << a.value << "\"";
    					first = false;
    				}
    				cout << "}" << endl;
    			}
    			entity.clear();
    		}

    		entity.push_back(g);
    		continue;
    	}

    }
	cout << g.groupcode << " : " << g.value << endl;

    fs.close();
    return EXIT_SUCCESS;
};

} /* namespace dxf */
