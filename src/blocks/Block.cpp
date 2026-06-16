/*
 * Block.cpp
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#include "blocks/Block.h"
#include <rapidjson/writer.h>

using std::to_string;

namespace dxf {


Block::~Block() {
	// TODO Auto-generated destructor stub
	// TOTO delete all entities from entities_ vector
}

void Block::initBlock(const vector<Group> &properties) {
	for (unsigned int i = 0; i < properties.size(); ++ i) {
		Group g = properties[i];
		switch (g.groupcode) {
			case 5:	// Handle
				this->handle_ = g.value;
				break;
				/*
					Start of application-defined group “{application_name”. For example, “{ACAD_REACTORS” indicates the start of the AutoCAD persistent reactors group (optional)
					102
					application- Codes and values within the 102 groups are application defined (optional)
					defined codes
					102 End of group, “}” (optional)
				*/
			case 330:	// Soft-pointer ID/handle to owner object
				this->ownerHandle_ = g.value;
				break;
			case 100:	// Subclass marker (AcDbEntity)
				if (g.value == "AcDbEntity") {using std::to_string;
					this->subclassMarker_ = g.value;
				}
				else {
					this->blockBegin_ = g.value;
				}
				break;
			case 8:		// Layer name
				this->layerName_ = g.value;
				break;
			case 2:		// Block name
				this->blockName_ = g.value;
				break;
			case 70:	// Block-type flags (bit-coded values, may be combined):
				this->blockType_ = stoi(g.value);
				break;
			case 64:	// This definition is a referenced external reference (ignoredon input)
				break;
			case 10:	//  Base point; DXF: X value; APP: 3D point
				this->basePoint_.x_ = stod(g.value);
				break;
			case 20:	// 20, 30 DXF: Y and Z values of base point
				this->basePoint_.y_ = stod(g.value);
				break;
			case 30:	// 20, 30 DXF: Y and Z values of base point
				this->basePoint_.z_ = stod(g.value);
				break;
			case 3: 	// Block name
				this->blockName_ = g.value;
				break;
			case 1:		// Xref path name
				this->xrefPath_ = g.value;
				break;
			case 4:		// Block description (optional)
				this->description_ = g.value;
				break;
			default:
				break;
		}
	}
}

void Block::finalizeBlock(const vector<Group> &properties) {
	for (unsigned int i = 0; i < properties.size(); ++ i) {
		Group g = properties[i];
		switch (g.groupcode) {
			case 5:	// Handle
				this->handle_ = g.value;
				break;
				/*
					Start of application-defined group “{application_name”. For example, “{ACAD_REACTORS” indicates the start of the AutoCAD persistent reactors group (optional)
					102
					application- Codes and values within the 102 groups are application defined (optional)
					defined codes
					102 End of group, “}” (optional)
				*/
			case 330:	// Soft-pointer ID/handle to owner object
				this->ownerHandle_ = g.value;
				break;
			case 100:	// Subclass marker (AcDbEntity)
				if (g.value == "AcDbBlockEnd") {
					this->blockEnd_ = g.value;
				}
				break;
			default:
				break;
		}
	}
}

Entity* Block::addEntity(const vector<Group> &properties) {
	Entity* e = this->entityFactor_.create(properties);
	if (e != nullptr) {
		this->entities_.push_back(e);
	}
	return e;
}

void Block::resolve(const Tables& tables) {
	for (unsigned int i = 0; i < this->entities_.size(); ++i) {
		if (this->entities_[i] != nullptr) {
			this->entities_[i]->resolve(tables);
		}
	}
}

string Block::toString() {
	string s;

	s += "\"handle\" : \"" + this->handle_ + "\", ";
	s += "\"owner_handle\" : \"" + this->ownerHandle_ + "\", ";
	s += "\"subclass_marker\" : \"" + this->subclassMarker_ + "\", ";
	s += "\"block_begin\" : \"" + this-> blockBegin_ + "\", ";
	s += "\"layer_name\" : \"" + this->layerName_ + "\", ";
	s += "\"block_name\" : \"" + this->blockName_ + "\", ";
	s += "\"block_type\" : " + to_string(this->blockType_) + ", ";

	s += ", \"base_point\" : {" ;
	s += "\"x\" : " + to_string(this->basePoint_.x_) + ", ";
	s += "\"y\" : " + to_string(this->basePoint_.y_) + ", ";
	s += "\"z\" : " + to_string(this->basePoint_.z_) + "},";

	s += "\"xref_path\" : \"" + this->xrefPath_ + "\", ";
	s += "\"description\" : \"" + this->description_ + "\", ";
	s += "\"block_end\" : \"" + this->blockEnd_  + "\"";

	return s;
}

void Block::write_json_properties(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
    writer.Key("handle");          writer.String(this->handle_.c_str());
    writer.Key("owner_handle");    writer.String(this->ownerHandle_.c_str());
    writer.Key("subclass_marker"); writer.String(this->subclassMarker_.c_str());
    writer.Key("block_begin");     writer.String(this->blockBegin_.c_str());
    writer.Key("layer_name");      writer.String(this->layerName_.c_str());
    writer.Key("block_name");      writer.String(this->blockName_.c_str());
    writer.Key("block_type");      writer.Int(this->blockType_);

    // Nested Object: No manual tracking of { or } or nested commas!
    writer.Key("base_point");
    writer.StartObject();
    writer.Key("x");               writer.Double(this->basePoint_.x_);
    writer.Key("y");               writer.Double(this->basePoint_.y_);
    writer.Key("z");               writer.Double(this->basePoint_.z_);
    writer.EndObject();

    writer.Key("xref_path");       writer.String(this->xrefPath_.c_str());
    writer.Key("description");     writer.String(this->description_.c_str());
    writer.Key("block_end");       writer.String(this->blockEnd_.c_str());
}


void Block::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	this->write_json_properties(writer);

	writer.Key("entities");
	writer.StartArray();
	for (unsigned int i = 0; i < this->entities_.size(); ++i) {
		if (this->entities_[i] != nullptr) {
			this->entities_[i]->write_to_json_writer(writer);
		}
	}
	writer.EndArray();

	writer.EndObject();
}

string Block::to_json() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	this->write_to_json_writer(writer);
    return buffer.GetString();
}

} /* namespace dxf */
