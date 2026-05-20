/*
 * Text.h
 *
 *  Created on: 3 Dec 2024
 *      Author: hobbes
 */

#ifndef ENTITIES_TEXT_H_
#define ENTITIES_TEXT_H_

#include <pugixml.hpp>
#include "document/Entity.h"

namespace dxf {

class Text : public Entity {
	// The following group codes apply to text entities. In addition to the group
	// codes described here, see Common Group Codes for Entities on page 61. For
	// information about abbreviations and formatting used in this table, see
	// Formatting Conventions in This Reference on page 2.

	// Text group codes
	// Group code Description
	// 100 Subclass marker (AcDbText)
	double thickness_; // 39 Thickness (optional; default = 0)
	Point_ first_alignment_point_; // 10 First alignment point (in OCS)
	// 20, 30 DXF: Y and Z values of first alignment point (in OCS)
	double text_height_; // 40 Text height
	std::string default_value_; // 1 Default value (the string itself)
	double text_rotation_ = 0; // 50 Text rotation (optional; default = 0)
	double x_scale_factor_ = 1; // 41 Relative X scale factor—width (optional; default = 1)
	// This value is also adjusted when fit-type text is used
	double oblique_angle_ = 0; // 51 Oblique angle (optional; default = 0)
	std::string text_style_name_ = "STANDARD"; // 7 Text style name (optional, default = STANDARD)
	int text_generation_flags_ = 0; // 71 Text generation flags (optional, default = 0)
	// 2 = Text is backward (mirrored in X)
	// 4 = Text is upside down (mirrored in Y)
	int horizontal_justification_ = 0; // 72 Horizontal text justification type (optional, default = 0)
	// 0 = Left; 1 = Center; 2 = Right
	// 3 = Aligned (if vertical alignment = 0)
	// 4 = Middle (if vertical alignment = 0)
	// 5 = Fit (if vertical alignment = 0)
	// See the Group 72 and 73 integer codes table for clarification

	Point_ second_alignment_point_; // 11 Second alignment point (in OCS) (optional)
	// DXF: X value; APP: 3D point
	// This value is meaningful only if the value of a 72 or 73 group is nonzero (if the justification is
	// anything other than baseline/left)
	// 21, 31 DXF: Y and Z values of second alignment point (in OCS) (optional)
	Point_ extrusion_direction_; // 210 Extrusion direction (optional; default = 0, 0, 1)
	// 220, 230 DXF: Y and Z values of extrusion direction (optional)

	// 100 Subclass marker (AcDbText)
	int vertical_justification_ = 0; // 73 Vertical text justification type (optional, default = 0)
	// 0 = Baseline; 1 = Bottom; 2 = Middle; 3 = Top
	// See the Group 72 and 73 integer codes table for clarification

	// The following table describes the group codes 72 (horizontal alignment) and
	// 73 (vertical alignment) in greater detail.

	// Group 72 and 73 integer codes
	// Group 1 2 3 4 5
	// 72
	// 0
	// Group 73
	// 3 (top) TLeft TCenter TRight
	// 2 (middle) MLeft MCenter MRight
	// 1 (bottom) BLeft BCenter BRight
	// 0 (baseline) Left Center Right Aligned Middle Fit

	// If group 72 and/or 73 values are nonzero then the first alignment point values
	// are ignored and AutoCAD calculates new values based on the second alignment
	// point and the length and height of the text string itself (after applying the
	// text style). If the 72 and 73 values are zero or missing, then the second
	// alignment point is meaningless.


public:
	Text(const vector<Group> &properties);
	virtual ~Text();
};

} /* namespace dxf */

#endif /* ENTITIES_TEXT_H_ */
