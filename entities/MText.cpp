/*
 * MText.cpp
 *
 *  Created on: 21 Apr 2022
 *      Author: hobbes
 */

#include "entities/MText.h"

namespace dxf {

MText::MText(const vector<Group> &properties) :Entity(properties) {
	// TODO Auto-generated constructor stub

	/*
			Attdef group codes
			Group code Description
			0 Entity type (MTEXT)
			100 Subclass marker (AcDbEntity)
			67 Absent or zero indicates entity is in model space. 1 indicates entity is in paper space (optional)
			8 Layer name
			100 Subclass marker (AcDbMText)
			10 Insertion point
			DXF: X value; APP: 3D point
			20,30 DXF: Y and Z values of insertion point
			40 Nominal (initial) text height
			41 Reference rectangle width
			46 Defined annotation height
			71 Attachment point:
			1 = Top left; 2 = Top center; 3 = Top right
			4 = Middle left; 5 = Middle center; 6 = Middle right
			7 = Bottom left; 8 = Bottom center; 9 = Bottom right
			72 Drawing direction:
			1 = Left to right
			3 = Top to bottom
			5 = By style (the flow direction is inherited from the associated text style)
			1 Text string
			If the text string is less than 250 characters, all characters appear in group 1. If the text string
			is greater than 250 characters, the string is divided into 250-character chunks, which appear
			in one or more group 3 codes. If group 3 codes are used, the last group is a group 1 and has
			fewer than 250 characters.
			3 Additional text (always in 250-character chunks) (optional)
			7 DXF: X value; APP: 3D vectText style name (STANDARD if not provided) (optional)
			70 | Chapter 6 ENTITIES Section
			Attdef group codes
			Group code Description
			210 Extrusion direction (optional; default = 0, 0, 1)
			DXF: X value; APP: 3D vector
			220,230 DXF: Y and Z values of extrusion direction (optional)
			11 X-axis direction vector (in WCS)
			DXF: X value; APP: 3D vector
			21,31 DXF: Y and Z values of X-axis direction vector (in WCS)
			42 Horizontal width of the characters that make up the mtext entity.
			This value will always be equal to or less than the value of group code 41 (read-only, ignored
			if supplied).
			43 Vertical height of the mtext entity (read-only, ignored if supplied)
			50 Rotation angle in radians
			73 Mtext line spacing style (optional):
			1 = At least (taller characters will override)
			2 = Exact (taller characters will not override)
			44 Mtext line spacing factor (optional):
			Percentage of default (3-on-5) line spacing to be applied.
			Valid values range from 0.25 to 4.00
			90 Background fill setting:
			0 = Background fill off
			1 = Use background fill color
			2 = Use drawing window color as background fill color
			63 Background color (if color index number)
			420-429 Background color (if RGB color)
			430-439 Background color (if color name)
			45 Fill box scale (optional):
			Determines how much border is around the text.
			ATTDEF | 71
			Attdef group codes
			Group code Description
			63 Background fill color (optional):
			Color to use for background fill when group code 90 is 1.
			441 Transparency of background fill color (not implemented)
			If group 72 and/or 74 values are nonzero then the first alignment point values
			are ignored and new values are calculated by AutoCAD, based on the second
			alignment point and the length and height of the text string itself (after
			applying the text style). If the 72 and 74 values are zero or missing, then the
			second alignment point is meaningless.
			*/

}

MText::~MText() {
	// TODO Auto-generated destructor stub
}

} /* namespace dxf */
