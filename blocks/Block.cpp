/*
 * Block.cpp
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#include "blocks/Block.h"

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
				break;using std::to_string;
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

void Block::addEntity(const vector<Group> &properties) {
	this->entityFactor_.create(properties);
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

	// Entities
//	vector<Entity*> entities_;

	return s;
}

string Block::toJson() {
	string s = "{" + toString() + ", \"entities\" : [";
	for (unsigned int i = 0; i <  this->entities_.size(); ++i) {
		s += this->entities_[i]->toJson();
	}
	s += "]}";
	return s;
}


} /* namespace dxf */
