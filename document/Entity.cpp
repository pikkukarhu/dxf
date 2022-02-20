/*
 * Entity.cpp
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#include "Entity.h"
#include <cstdint>

#include "file/File.h"


using std::stoi;
using std::stod;

namespace dxf {

Entity::Entity(const vector<Group> &properties) {
	// TODO Auto-generated constructor stub

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
				this->subclassMarker_ = g.value;
				break;
			case 67:	//Absent or zero indicates entity is in model space. 1 indicates entity is in 0 paper space (optional).
				this->modelSpace_ = (g.value == "1" ? true : false);
				break;
			case 410:	// APP: layout tab name not omitted
				break;
			case 8:		// Layer name not omitted
				this->layer_ = g.value;
				break;
			case 6:	// Linetype name (present if not BYLAYER). The special name BYBLOCK in- BYLAYER
					// dicates a floating linetype (optional)
				this->lineType_ = g.value;
				break;
			case 347:	// Hard-pointer ID/handle to material object (present if not BYLAYER) BYLAYER
				break;
				// Color number (present if not BYLAYER); zero indicates the BYBLOCK BYLAYER
				// (floating) color; 256 indicates BYLAYER; a negative value indicates that
				// the layer is turned off (optional); colorNumber_ default is 256
			case 62:
				this->colorNumber_ = stoi(g.value);	// 0 = bylayer,
				break;
			case 370:	// Lineweight enum value. Stored and moved around as a 16-bit integer. not omitted
				this->lineWeight_ = stoi(g.value);
				break;
			case 48:	// Linetype scale (optional) 1.0
				this->linetypeScale_ = stod(g.value);
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
					this->rgb_.blue_ = 0xFF & c;
					this->rgb_.green_ = (0xFF << 8) & c;
					this->rgb_.red_ = (0xFF << 16) & c;
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
				this->shadowMode_ = stoi(g.value);
				break;
			default:
				break;
		}
	} // Fod loop

}

Entity::~Entity() {
	// TODO Auto-generated destructor stub
}

/*
Entity::Entity(const Entity &other) {
	// TODO Auto-generated constructor stub

}

Entity::Entity(Entity &&other) {
	// TODO Auto-generated constructor stub
}
*/

} /* namespace dxf */
