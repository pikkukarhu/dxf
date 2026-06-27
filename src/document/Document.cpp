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

#include <map>
#include "entries/Layer.h"
#include "file/ACIConverter.h"
#include "EntityFactory.h"
#include "file/File.h"
#include "tables/Table.h"

using std::cout;
using std::endl;
using std::cerr;


namespace dxf {


Document::Document(string file, bool isBlackBackground) : is_black_background_(isBlackBackground) {
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
	this->entities_.resolve(this->tables_, is_black_background_);
	this->blocks_.resolve(this->tables_, is_black_background_);

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

	BoundingBox bb = entities_.get_bounding_box();

    //if (bbox_initialized_) {
    double width = bb.width();
    double height = bb.height();
        
	min_x_ = bb.min_x;
	min_y_ = bb.min_y;

    // Add 5% margin
    double margin_x = width == 0 ? 10 : width * 0.05;
    double margin_y = height == 0 ? 10 : height * 0.05;


    string viewBox = std::to_string(min_x_ - margin_x) + " " + 
                    std::to_string((min_y_ - margin_y)/*- height*/) + " " + 
                    std::to_string(width + 2 * margin_x) + " " + 
                    std::to_string(height + 2 * margin_y);
        
    svg.append_attribute("viewBox").set_value(viewBox.c_str());
	svg.append_attribute("transform").set_value("scale(1, -1)");

	svg.append_attribute("width").set_value("100%");
	svg.append_attribute("height").set_value("100%");
    
	// Flip Y axis to match DXF coordinate system
	// Check if we show paper-space or model-space entities, and select bounding box accordingly 
	// FIXME first we do only modelspace, so we can use overall bounding box, but if we want to support paperspace, 
	//we need to calculate bounding box for each separately and flip only modelspace

    string style_attr = "background-color: " + string(is_black_background_ ? "black" : "white");
	svg.append_attribute("style").set_value(style_attr.c_str());

	svg.append_attribute("xmlns").set_value("http://www.w3.org/2000/svg");

    // Create groups for layers
    std::map<string, pugi::xml_node> layer_groups;
    Table* layerTable = tables_.getTable("LAYER");
    if (layerTable != nullptr) {
        for (auto entry : layerTable->getEntries()) {
            Layer* l = dynamic_cast<Layer*>(entry);
            if (l != nullptr) {
                pugi::xml_node g = svg.append_child("g");
                g.append_attribute("id").set_value(l->getName().c_str());
                
                RGB color = ACIConverter::aciToRgb(l->getColorNumber(), is_black_background_);
                char hex[8];
                sprintf(hex, "#%02x%02x%02x", color.red, color.green, color.blue);
                g.append_attribute("stroke").set_value(hex);
                g.append_attribute("fill").set_value(hex); // Set fill as well for text inheritance
                
                // AutoCAD lineweight is in hundredths of millimeters
                if (l->getLineWeight() > 0) {
                    double sw = l->getLineWeight() / 100.0;
                    g.append_attribute("stroke-width").set_value(sw);
                }

                layer_groups[l->getName()] = g;
            }
        }
    }

	entities_.to_svg(svg, is_black_background_, layer_groups);
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
