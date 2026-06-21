/*
 * Entities.cpp
 *
 *  Created on: 17 Feb 2022
 *      Author: hobbes
 */

#include "Document.h"

#include <fstream>
#include <iostream>
#include <cctype>

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

static string make_svg_reference_id(const string& value) {
    string id;
    for (char c : value) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalnum(uc) || c == '_' || c == '-' || c == '.') {
            id += c;
        } else {
            id += '_';
        }
    }
    return id.empty() ? "layer" : id;
}

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
    
    string style_attr = "background-color: " + string(is_black_background_ ? "black" : "white");
	svg.append_attribute("style").set_value(style_attr.c_str());

	svg.append_attribute("xmlns").set_value("http://www.w3.org/2000/svg");

    pugi::xml_node defs = svg.append_child("defs");
    pugi::xml_node model_space = svg.append_child("g");
    model_space.append_attribute("id").set_value("model__space");
    model_space.append_attribute("display").set_value("inline");
    pugi::xml_node paper_space = svg.append_child("g");
    paper_space.append_attribute("id").set_value("paper_space");
    paper_space.append_attribute("display").set_value("none");

    // Create layer definitions and reference them from model/paper space groups.
    std::map<string, Entities::SvgLayerGroups> layer_groups;
    Table* layerTable = tables_.getTable("LAYER");
    if (layerTable != nullptr) {
        for (auto entry : layerTable->getEntries()) {
            Layer* l = dynamic_cast<Layer*>(entry);
            if (l != nullptr) {
                string layer_ref_id = make_svg_reference_id(l->getName());
                string model_layer_id = layer_ref_id + "__model_space";
                string paper_layer_id = layer_ref_id + "__paper_space";

                pugi::xml_node layer = defs.append_child("g");
                layer.append_attribute("id").set_value(l->getName().c_str());

                pugi::xml_node model_layer = layer.append_child("g");
                model_layer.append_attribute("id").set_value(model_layer_id.c_str());
                model_layer.append_attribute("data-layer").set_value(l->getName().c_str());

                pugi::xml_node paper_layer = layer.append_child("g");
                paper_layer.append_attribute("id").set_value(paper_layer_id.c_str());
                paper_layer.append_attribute("data-layer").set_value(l->getName().c_str());
                
                RGB color = ACIConverter::aciToRgb(l->getColorNumber(), is_black_background_);
                char hex[8];
                sprintf(hex, "#%02x%02x%02x", color.red, color.green, color.blue);
                model_layer.append_attribute("stroke").set_value(hex);
                model_layer.append_attribute("fill").set_value(hex); // Set fill as well for text inheritance
                paper_layer.append_attribute("stroke").set_value(hex);
                paper_layer.append_attribute("fill").set_value(hex);
                
                // AutoCAD lineweight is in hundredths of millimeters
                if (l->getLineWeight() > 0) {
                    double sw = l->getLineWeight() / 100.0;
                    model_layer.append_attribute("stroke-width").set_value(sw);
                    paper_layer.append_attribute("stroke-width").set_value(sw);
                }

                pugi::xml_node model_use = model_space.append_child("use");
                string model_href = "#" + model_layer_id;
                model_use.append_attribute("href").set_value(model_href.c_str());

                pugi::xml_node paper_use = paper_space.append_child("use");
                string paper_href = "#" + paper_layer_id;
                paper_use.append_attribute("href").set_value(paper_href.c_str());

                layer_groups[l->getName()] = {model_layer, paper_layer};
            }
        }
    }

	entities_.to_svg(model_space, paper_space, is_black_background_, layer_groups);
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
