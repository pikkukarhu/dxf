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
    		if (section == "HEADER") {
    			this->readHeader(&f);
    		}
    		else if (section == "TABLES") {
    			this->tables_.read(&f);
    		}
    		else if (section == "ENTITIES") {
    			this->entities_.read(&f);
    			//this->entities_.write_json("entities.json");
				//export_svg("entities.svg");
    		}
    		else if (section == "BLOCKS") {
    			this->blocks_.read(&f);
    		}
    		else {	// Default read next section to dev/null
    			Section sec;
    			sec.read(&f);
    		}
    	}
    	//export_svg("entities.svg");
    }

	// Resolve all properties (BYLAYER, etc.) after tables and entities are read
	this->entities_.resolve(this->tables_);
	this->blocks_.resolve(this->tables_);

    // Update drawing extents
    BoundingBox bb = entities_.get_bounding_box();
    if (bb.initialized) {
        min_x_ = bb.min_x;
        max_x_ = bb.max_x;
        min_y_ = bb.min_y;
        max_y_ = bb.max_y;
        bbox_initialized_ = true;
    }
}

Document::~Document() {
}

void Document::readHeader(File* f) {
	Group g;
    string version = "";
    bool utf8_forced = false;

	while (f->readGroup(g)) {
		if (g.groupcode == 0 && g.value == "ENDSEC") {
			cout << "] ENDSEC" << endl;
			return;
		}
        if (g.groupcode == 9 && g.value == "$ACADVER") {
            if (f->readGroup(g)) {
                version = g.value;
                // AC1021 (R2007) and newer use UTF-8
                if (version >= "AC1021") {
                    f->setCodepage("UTF-8");
                    utf8_forced = true;
                }
            }
        }
		if (g.groupcode == 9 && g.value == "$DWGCODEPAGE") {
			if (f->readGroup(g)) {
                if (!utf8_forced) {
				    f->setCodepage(g.value);
                }
			}
		}
	}
}

void Document::export_svg(const std::string& file) {
	pugi::xml_document doc;
	pugi::xml_node svg = doc.append_child("svg");

    if (bbox_initialized_) {
        double width = max_x_ - min_x_;
        double height = max_y_ - min_y_;
        
        // Add 5% margin
        double margin_x = width * 0.05;
        double margin_y = height * 0.05;
        if (width == 0) margin_x = 10;
        if (height == 0) margin_y = 10;

        string viewBox = std::to_string(min_x_ - margin_x) + " " + 
                         std::to_string(min_y_ - margin_y) + " " + 
                         std::to_string(width + 2 * margin_x) + " " + 
                         std::to_string(height + 2 * margin_y);
        
        svg.append_attribute("viewBox").set_value(viewBox.c_str());
    }

	svg.append_attribute("width").set_value("100%");
	svg.append_attribute("height").set_value("100%");
	svg.append_attribute("style").set_value("background-color: black");

	svg.append_attribute("xmlns").set_value("http://www.w3.org/2000/svg");

	    // Create a group element for rotation
	entities_.to_svg(svg);
	// Save the XML document to a file
	doc.save_file(file.c_str());
}

void Document::export_json(const std::string& file) {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	writer.StartObject();
	writer.Key("tables");
	tables_.write_to_json_writer(writer);
	writer.Key("blocks");
	blocks_.write_to_json_writer(writer);
	writer.Key("entities");
	entities_.write_to_json_writer(writer);
	writer.EndObject();

	std::ofstream outputFile(file);
	if (!outputFile.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
		return;
	}
	outputFile << buffer.GetString() << std::endl;
	outputFile.close();
}

} /* namespace dxf */
