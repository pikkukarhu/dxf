/*
 * AttDef.cpp
 *
 *  Created on: 19 Apr 2022
 *      Author: hobbes
 */

#include "entities/AttDef.h"

namespace dxf {

AttDef::AttDef(const vector<Group> &properties) : Entity(properties) {

	for (unsigned int i = 0; i < properties.size(); ++i) {

		Group g = properties[i];
		switch (g.groupcode) {


		case 100: 	// Subclass marker (AcDbText)
//			this->subclassMarker_ = g.value;
			break;
		case 39:	// Thickness (optional; default = 0)
			break;
		case 10: 	// First alignment point (in OCS)
			//DXF: X value; APP: 3D point
			// Start point (in WCS)
			this->aligmentPt_.x_ = stod(g.value);
			break;
		case 20:	// Y value,
			this->aligmentPt_.y_ = stod(g.value);
			break;
		case 30: 	// Z values of text start point (in OCS)
			this->aligmentPt_.z_ = stod(g.value);
			//	10 Alignment point of attribute or attribute definition
			//	DXF: X value; APP: 3D point
			//	20,30 DXF: Y and Z values of insertion point
			break;
		case 40:	// Text height
			// current annotation scale
			this->annotationScale_ = stod(g.value);
			break;
		case 1:		// Default value (string)
			break;
		case 50:	// Text rotation (optional; default = 0

			break;
		case 41:	//Relative X scale factor (width) (optional; default = 1). This value is also adjusted when fit-type
					//text is used
			break;
		case 51:	// Oblique angle (optional; default = 0)
			break;
		case 7:		// Text style name (optional; default = STANDARD)
			break;
		case 71:	// Text generation flags (optional; default = 0); see TEXT on page 144 group codes
			break;
		case 72:	// Horizontal text justification type (optional; default = 0); see TEXT on page 144 group codes
			break;
		case 11:	// Second alignment point (in OCS) (optional)
					// DXF: X value; APP: 3D point
					// Meaningful only if 72 or 74 group values are nonzero
		case 21: 	// 21, 31 DXF: Y and Z values of second alignment point (in OCS) (optional)
			break;
		case 31:
			break;
		case 210:	// Extrusion direction (optional; default = 0, 0, 1)
					//DXF: X value; APP: 3D vector
			break;
		case 220:	//220, 230 DXF: Y and Z values of extrusion direction
			break;
		case 230:
			break;
		case 280:	// Version number:
					// 0 = 2010
			// Lock position flag. Locks the position of the attribute within the block reference
			this->lockPositionFlag_ = stoi(g.value);
			this->dublicateRecordCloningFlag_ = stoi(g.value); //1 = Keep existing
			break;
		case 3:		// Prompt string
			break;
		case 2:		// Tag string (cannot contain spaces)
			this->tag_ = g.value;
			// attribute or attribute definition tag string
			// ATTDEF | 69
			this->tagString_ = g.value;
			break;
		case 70:	// Attribute flags:
					// 1 = Attribute is invisible (does not appear)
					// 2 = This is a constant attribute
					// 4 = Verification is required on input of this attribute
					// 8 = Attribute is preset (no prompt during insertion)
			this->flags_ = stoi(g.value);
					// MText flag:
					// 2 = multiline attribute
					// 4 = constant multiline attribute definition
			this->mtextFlag_ = stoi(g.value);
					// isReallyLocked flag:
					// 0 = unlocked
					// 1 = locked
			this->isReallyLockedFlag_ = stoi(g.value);
					//  Number of secondary attributes or attribute definitions
			this->numOfSecondaryAttributes_ = stoi(g.value);
			break;
		case 73:	// Field length (optional; default = 0) (not currently used)
			this->fieldLength_ = stoi(g.value);
			break;
		case 74:	// Vertical text justification type (optional, default = 0); see group code 73 inTEXT on page 144
			this->verticalTestJustification_ = stoi(g.value);
			break;
		case 340:	// hard-pointer id of secondary attribute(s) or attribute definition(s)
			this->secondaryAttributePointer_ = g.value;
			break;
		default:
			break;
		}
	}
}

AttDef::~AttDef() {
	// TODO Auto-generated destructor stub
}

} /* namespace dxf */
