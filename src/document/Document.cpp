/*
 * Entities.cpp
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#include "Document.h"

#include <fstream>
#include <iostream>

#include <pugixml.hpp>

#include "EntityFactory.h"
#include "file/File.h"
#include "tables/Table.h"

using std::cout;
using std::endl;
using std::cerr;


namespace dxf {


Document::Document(string file) {
	File f(file);

    bool sectionStart = false;

    string section;
/*
    vector<Group> entity;		// Buffer of one entitity lines
    vector<Group> table;
*/
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
    			this->entities_.write_json("entities.json");
    		}
    		else if (section == "BLOCKS") {
    			this->blocks_.read(&f);
    		}
    		else {	// Default read next section to dev/null
    			Section sec;
    			sec.read(&f);
    		}
    	}
    	export_svg("entities.svg");
    }
}

Document::~Document() {
}

void Document::export_svg(const std::string& file) {
	pugi::xml_document doc;
	pugi::xml_node svg = doc.append_child("svg");

	svg.append_attribute("width").set_value("1000");
	svg.append_attribute("height").set_value("1000");
	svg.append_attribute("xmlns").set_value("http://www.w3.org/2000/svg");

	    // Create a group element for rotation
	entities_.to_svg(svg);
	// Save the XML document to a file
	doc.save_file(file.c_str());
}

void Document::export_json(const std::string& file) {

	std::ofstream outputFile(file);
	// Check if the file is successfully opened
	if (!outputFile.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
		return;
	}
	outputFile << "{ \"blocks\" : " << std::endl << blocks_.to_json() << ", " << std::endl;
	outputFile << "{ \"entities\" : " << std::endl << entities_.to_json() << " }" << std::endl;
	outputFile.close();
}

} /* namespace dxf */
