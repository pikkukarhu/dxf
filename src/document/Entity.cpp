/*
 * Entity.cpp
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#include "Entity.h"
#include <cstdint>

#include "file/File.h"
#include "tables/Tables.h"
#include "tables/Table.h"
#include "entries/Layer.h"
#include "file/ACIConverter.h"


using std::stoi;
using std::stod;
using std::to_string;

namespace dxf {

bool Entity::show_bounding_box = false;
int Entity::next_entity_id = 1;

Entity::Entity(const vector<Group> &properties) {

	for (unsigned int i = 0; i < properties.size(); ++ i) {
		Group g = properties[i];
		switch (g.groupcode) {
			case -1: 	// APP: entity name (changes each time a drawing is opened) not omitted
				break;
			case 61:
				break;	// Group codes that apply to all graphical objects. If omitted, defaults to… Group code Description
			case 0: 	// Entity type not omitted
				this->type_ = g.value;
				break;
			case 5:		// Handle not omitted
				this->handle_ = g.value;
				break;

				/*
				 * 	102 Start of application-defined group no default
				 * “{application_name” (optional)
				 * application- Codes and values within the 102 groups are application-defined (optional) no default
				 * defined codes
				 * 102 End of group, “}” (optional) no default
				 * “{ACAD_REACTORS” indicates the start of the AutoCAD persistent reactors no default
				 * group. This group exists only if persistent reactors have been attached
				 * to this object (optional)
				 * 102
				 * 330 Soft-pointer ID/handle to owner dictionary (optional) no default
				 * 102 End of group, “}” (optional) no default
				 * “{ACAD_XDICTIONARY” indicates the start of an extension dictionary no default
				 * group. This group exists only if an extension dictionary has been attached
				 * to the object (optional)
				 * 102
				 * 360 Hard-owner ID/handle to owner dictionary (optional) no default
				 * 102 End of group, “}” (optional) no default
				 * 330 Soft-pointer ID/handle to owner BLOCK_RECORD object not omitted
				 *
				 * Thesa are ignored now
				 */
			case 102:
			case 330:
			case 360:
				break;
			case 100:	// Subclass marker (AcDbEntity) not omitted
				this->subclass_marker_ = g.value;
				break;
			case 67:	//Absent or zero indicates entity is in model space. 1 indicates entity is in 0 paper space (optional).
				this->model_space_ = (g.value == "1" ? true : false);
				break;
			case 410:	// APP: layout tab name not omitted
				break;
			case 8:		// Layer name not omitted
				this->layer_ = g.value;
				break;
			case 6:	// Linetype name (present if not BYLAYER). The special name BYBLOCK in- BYLAYER
					// dicates a floating linetype (optional)
				this->line_type_ = g.value;
				break;
			case 347:	// Hard-pointer ID/handle to material object (present if not BYLAYER) BYLAYER
				break;
				// Color number (present if not BYLAYER); zero indicates the BYBLOCK BYLAYER
				// (floating) color; 256 indicates BYLAYER; a negative value indicates that
				// the layer is turned offto_string( (optional); colorNumber_ default is 256
			case 62:
				this->color_number_ = stoi(g.value);	// 0 = bylayer,
				break;
			case 370:	// Lineweight enum value. Stored and moved around as a 16-bit integer. not omitted
				this->line_weight_ = stoi(g.value);
				break;
			case 48:	// Linetype scale (optional) 1.0
				this->linetype_scale_ = stod(g.value);
				break;
			case 60:	// Object visibility (optional): 0 = Visible; 1 = Invisible 0; default visible(?)
				this->visible_ = (g.value == "0");
				break;
				/*
				 * Number of bytes in the proxy entity graphics represented in the sub- no default
				 * sequent 310 groups, which are binary chunk records (optional)
				 */
			case 92:
				break;
			case 310: // Proxy entity graphics data (multiple lines; 256 characters max. per line) no default (optional)
				break;
			case 420: {

					// A 24-bit color value that should be dealt with in terms of bytes with values no default
					// of 0 to 255. The lowest byte is the blue value, the middle byte is the
					// green value, and the third byte is the red value. The top byte is always
					// 0. The group code cannot be used by custom entities for their own data
					// because the group code is reserved for AcDbEntity, class-level color data
					// and AcDbEntity, class-level transparency data
					// Color name. The group code cannot be used by custom entities for their no default
					// own data because the group code is reserved for AcDbEntity, class-level
					// color data and AcDbEntity, class-level transparency data

					int32_t c = (int32_t)stol(g.value);
					this->rgb_.blue = 0xFF & c;
					this->rgb_.green = (0xFF << 8) & c;
					this->rgb_.red = (0xFF << 16) & c;
				}
				break;
					/* Transparency value. The group code cannot be used by custom entities no default
					 * for their own data because the group code is reserved for AcDbEntity,
					 * class-level color data and AcDbEntity, class-level transparency data
					 *
					 */
			case 430:
			case 440:
				break;
			case 390:	// Hard-pointer ID/handle to the plot style object no default
				break;
			case 284:	//  Shadow mode no default
					// 0 = Casts and receives shadows
					// 1 = Casts shadows
					// 2 = Receives shadows
					// 3 = Ignores shadows
				this->shadow_mode_ = stoi(g.value);
				break;
			default:
				break;
		}
	} // For loop

	this->id_ = Entity::next_entity_id++;
	this->readable_id_ = this->type_ + "-" + std::to_string(this->id_);
}

Entity::~Entity() {
	// TODO Auto-generated destructor stub
}


string Entity::to_string() {
	string s;

	s += "\"id\" : " + std::to_string(this->id_) + ", ";
	s += "\"readable_id\" : \"" + this->readable_id_ + "\", ";
	s += "\"handle\" : \"" + this->handle_ + "\", ";
	s += "\"line_style\" : \"" + this->line_style_ + "\", ";
	s += "\"subclass_marker\" : \"" + this->subclass_marker_ + "\", ";
	s += "\"model_space\" : " + std::to_string(this->model_space_) + ", ";
	s += "\"layer\" : \"" + this->layer_ + "\", ";
	s += "\"line_type\" : \"" + this->line_type_ + "\", ";
	s += "\"color_number\" : " + std::to_string(this->color_number_) + ", ";
	s += "\"line_weight\" : " + std::to_string(this->line_weight_) + ", ";
	s += "\"linetype_scale\" : " + std::to_string(this->linetype_scale_) + ", ";
	s += "\"visible\" : " + std::to_string(this->visible_) + ", ";
	s += "\"color_rgb\" : {" ;
		s += "\"red\" : " + std::to_string(this->rgb_.red) + ", ";
		s += "\"green\" : " + std::to_string(this->rgb_.green) + ", ";
		s += "\"blue\" : " + std::to_string(this->rgb_.blue);
	s += "}, ";
	s += "\"shadow_mode\" : " + std::to_string(this->shadow_mode_);

	return s;
}

string Entity::get_svg_color() {
    if (this->color_number_ == 256) {
        return ""; // Inherit from group
    }
	char hex[8];
	sprintf(hex, "#%02x%02x%02x", resolved_rgb_.red, resolved_rgb_.green, resolved_rgb_.blue);
	return string(hex);
}


void Entity::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.Key("id");              writer.Int(this->id_);
	writer.Key("readable_id");     writer.String(this->readable_id_.c_str());
	writer.Key("handle");          writer.String(this->handle_.c_str());
	writer.Key("type");            writer.String(this->type_.c_str());
	writer.Key("layer");           writer.String(this->layer_.c_str());
	writer.Key("line_type");       writer.String(this->line_type_.c_str());
	writer.Key("color_number");    writer.Int(this->color_number_);
	writer.Key("line_weight");     writer.Int(this->line_weight_);
	writer.Key("visible");         writer.Bool(this->visible_);
	
	writer.Key("color_rgb");
	writer.StartObject();
	writer.Key("red");             writer.Int(this->rgb_.red);
	writer.Key("green");           writer.Int(this->rgb_.green);
	writer.Key("blue");            writer.Int(this->rgb_.blue);
	writer.EndObject();

	writer.Key("resolved_color_number"); writer.Int(this->resolved_color_);
	writer.Key("resolved_linetype");     writer.String(this->resolved_linetype_.c_str());
	writer.Key("resolved_line_weight");  writer.Int(this->resolved_line_weight_);
	writer.Key("resolved_rgb");
	writer.StartObject();
	writer.Key("red");                   writer.Int(this->resolved_rgb_.red);
	writer.Key("green");                 writer.Int(this->resolved_rgb_.green);
	writer.Key("blue");                  writer.Int(this->resolved_rgb_.blue);
	writer.EndObject();
}

void Entity::draw_bounding_box(pugi::xml_node& svg_node) {
	if (!show_bounding_box) return;

	auto rect = svg_node.append_child("rect");
	rect.append_attribute("x").set_value(bounding_box_.x);
	rect.append_attribute("y").set_value(bounding_box_.y);
	rect.append_attribute("width").set_value(bounding_box_.width);
	rect.append_attribute("height").set_value(bounding_box_.heigth);
	rect.append_attribute("fill").set_value("none");
	rect.append_attribute("stroke").set_value("blue");
	rect.append_attribute("stroke-width").set_value("2px");
	rect.append_attribute("vector-effect").set_value("non-scaling-stroke");
}

void Entity::add_common_svg_attributes(pugi::xml_node& node) {
	node.append_attribute("id").set_value(this->readable_id_.c_str());
	string color = get_svg_color();
	if (string(node.name()) == "text") {
		if (!color.empty()) {
			node.append_attribute("fill").set_value(color.c_str());
		}
	} else {
		node.append_attribute("fill").set_value("none");
		if (!color.empty()) {
			node.append_attribute("stroke").set_value(color.c_str());
		}
	}
}

void Entity::resolve(const Tables& tables, bool isBlackBackground) {
	// 1. Resolve Color
	if (this->color_number_ == 256) { // BYLAYER
		Table* layerTable = tables.getTable("LAYER");
		if (layerTable != nullptr) {
			Layer* layer = dynamic_cast<Layer*>(layerTable->getEntry(this->layer_));
			if (layer != nullptr) {
				this->resolved_color_ = layer->getColorNumber();
			}
		}
	} else {
		this->resolved_color_ = this->color_number_;
	}

	// 2. Resolve RGB
	if (this->rgb_.red != 0 || this->rgb_.green != 0 || this->rgb_.blue != 0) {
		this->resolved_rgb_ = this->rgb_;
	} else {
		this->resolved_rgb_ = ACIConverter::aciToRgb(this->resolved_color_, isBlackBackground);
	}

	// 3. Resolve Linetype
	if (this->line_type_ == "BYLAYER") {
		Table* layerTable = tables.getTable("LAYER");
		if (layerTable != nullptr) {
			Layer* layer = dynamic_cast<Layer*>(layerTable->getEntry(this->layer_));
			if (layer != nullptr) {
				this->resolved_linetype_ = layer->getLinetypeName();
			}
		}
	} else {
		this->resolved_linetype_ = this->line_type_;
	}

	// 4. Resolve Lineweight
	if (this->line_weight_ == -1) { // BYLAYER (usually -1 in DXF for entities)
		Table* layerTable = tables.getTable("LAYER");
		if (layerTable != nullptr) {
			Layer* layer = dynamic_cast<Layer*>(layerTable->getEntry(this->layer_));
			if (layer != nullptr) {
				this->resolved_line_weight_ = layer->getLineWeight();
			}
		}
	} else {
		this->resolved_line_weight_ = this->line_weight_;
	}
}

} /* namespace dxf */
