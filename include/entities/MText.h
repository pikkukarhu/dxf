/*
 * MText.h
 *
 *  Created on: 21 Apr 2022
 *      Author: hobbes
 */

#ifndef ENTITIES_MTEXT_H_
#define ENTITIES_MTEXT_H_

#include <cmath>
#include "document/Entity.h"

namespace dxf {

enum class AttachmentPoints {
    TopLeft = 1,       // 1 = Top left
    TopCenter = 2,     // 2 = Top center
    TopRight = 3,      // 3 = Top right
    MiddleLeft = 4,    // 4 = Middle left
    MiddleCenter = 5,  // 5 = Middle center
    MiddleRight = 6,   // 6 = Middle right
    BottomLeft = 7,    // 7 = Bottom left
    BottomCenter = 8,  // 8 = Bottom center
    BottomRight = 9    // 9 = Bottom right
};

enum class DrawingDirection {
    LeftToRight = 1,    // 1 = Left to right
    TopToBottom = 3,    // 3 = Top to bottom
    ByStyle = 5         // 5 = By style (the flow direction is inherited from the associated text style)
};

enum class MtextLineSpacingStyle {
	AtLeast = 1, 		// 1 = At least (taller characters will override)
	Exact = 2 			// 2 = Exact (taller characters will not override)
};

enum class BackgroundFillSetting {
	BackgroundFillOff = 0,
	UseBackgroundFillColor = 1,
	UseDrawingWindowColor = 2
};

class MText final : public Entity {
private:
	inline RGB to_rgb(long color) {
		int32_t c = (int32_t)color;
		RGB rgb;
		rgb.blue = 0xFF & c;
		rgb.green = (0xFF << 8) & c;
		rgb.red = (0xFF << 16) & c;

		return rgb;
	}
	/* The following group codes apply to mtext entities. In addition to the group */
	/* codes described here, see Common Group Codes for Entities on page 61. For */
	/* information about abbreviations and formatting used in this table, see */
	/* Formatting Conventions in This Reference on page 2. */
	/* Mtext group codes */
	/* Group code Description */
	std::string subclass_marker_; // (AcDbMText)
	Point_ insertion_point_; /*
	10 Insertion point
	DXF: X value; APP: 3D point
	20, 30 DXF: Y and Z values of insertion point */
	double nominal_text_height_; // 40 Nominal (initial) text height
	double reference_rectangle_width_; // 41 Reference rectangle width

	AttachmentPoints attachment_point_; // group code 71
	DrawingDirection drawing_direction_; // group code 72

	/* Text string. If the text string is less than 250 characters, all characters appear in group 1. If the */
	/* text string is greater than 250 characters, the string is divided into 250-character chunks, which */
	/* appear in one or more group 3 codes. If group 3 codes are used, the last group is a group 1 */
	/* and has fewer than 250 characters */
	std::string text_;
	// std::string additional_text_;//  3 Additional text (always in 250-character chunks) (optional)



	std::string text_style_name_; // (STANDARD if not provided) (optional)
	Point_ extrusion_direction_; /*
		210 Extrusion direction (optional; default = 0, 0, 1)
		DXF: X value; APP: 3D vector
		220, 230 DXF: Y and Z values of extrusion direction (optional) */
	Point_ x_axis_direction_vector_; /*
		11 X-axis direction vector (in WCS)
		DXF: X value; APP: 3D vector
		A group code 50 (rotation angle in radians) passed as DXF input is converted to the equivalent
		direction vector (if both a code 50 and codes 11, 21, 31 are passed, the last one wins). This is
		provided as a convenience for conversions from text objects
		21, 31 DXF: Y and Z values of X-axis direction vector (in WCS)
		:: x_axis_direction_vector_ = Point_(cos(rotation), sin(rotation), 0);
		*/

	double max_horizontal_width_;				// 41
	double horizontal_width_of_the_characters_; // 42
		/* Horizontal width of the characters that make up the mtext entity. This value will always be */
		/* equal to or less than the value of group code 41 (read-only, ignored if supplied) */
	double vertical_height_of_mtext_entity_;
		/* 43 Vertical height of the mtext entity (read-only, ignored if supplied) */
	double rotation_angle_; // 50 Rotation angle in radians:: Use as :: x_axis_direction_vector_ = Point_(cos(rotation), sin(rotation), 0);
	MtextLineSpacingStyle mtext_line_spacing_style_;
		/* 73 Mtext line spacing style (optional):
			1 = At least (taller characters will override)
			2 = Exact (taller characters will not override)
		*/
	double mtext_line_spacing_factor_; // (optional):_,percentage_of_default_(3-on-5)_line_spacing_to_be_applied._valid_values_range_from_0.25_to_4.00,Mtext line spacing factor (optional): ,Percentage of default (3-on-5) line spacing to be applied. Valid values range from 0.25 to 4.00
	BackgroundFillSetting background_fill_setting_; //  90
	int background_color_;       //   (if_color_index_number),Background color (if color index number)
	RGB background_color_rgb_[10]; // 420 - 429 Background color (if RGB color)
	std::string background_color_name_[10];	// 430 - 439 Background color (if color name)
	double fill_box_scale_;		// 45 Fill box scale (optional):
	int background_fill_color_; // 63 Background fill color (optional):
	double transparency_of_background_;	// 441 Transparency of background fill color (not implemented)
	int column_type_;			// 75 Column type
	int column_count_;			// 76 Column count
	bool column_flow_reversed_;	// 78 Column Flow Reversed
	bool column_autoheight_ ;	// 79 Column Autoheight
	double column_width_;		// 48 Column width
	double column_gutter_;		// 49 Column gutter
	std::vector<double >column_heights_; // ,column_heights; this code is followed by a column count (Int16), and then the number of column heights

public:
	MText(const vector<Group> &properties);
	virtual ~MText();
};

} /* namespace dxf */

#endif /* ENTITIES_MTEXT_H_ */
