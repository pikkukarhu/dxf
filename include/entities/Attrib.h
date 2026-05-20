/*
 * Attrib.h
 *
 *  Created on: 5 Feb 2024
 *      Author: hobbes
 */

#ifndef ENTITIES_ATTRIB_H_
#define ENTITIES_ATTRIB_H_

#include "document/Entity.h"

namespace dxf {

#define CONST_ATTRIBUTE			0x2
#define VERIFICATION_REQUIRED 	0x4
#define PRESET_ATTRIBUTE 		0x8

class Attrib final : public Entity {
private:

	int version_number_;

	int generation_flags_ = 0;		// 71 Text generation flags (optional; default = 0). See TEXT on page 144 group codes
	int horizontal_justification_type_ = 0; // 72 Horizontal text justification type (optional; default = 0). See TEXT on page 144 group codes
	int vertical_justification_type_ = 0;	// 74 Vertical text justification type (optional; default = 0). See group code 73 inTEXT on page 144
	Point_ aligment_point_;			// 11 Alignment point (in OCS) (optional) \ DXF: X value; APP: 3D point \ Present only if 72 or 74 group is present and nonzero \ 21, 31 DXF: Y and Z values of alignment point (in OCS) (optional)
	Point_ extrusion_direction_;	// 210 Extrusion direction. Present only if the entity's extrusion direction is not parallel to the WCS Z \ axis (optional; default = 0, 0, 1) \ DXF: X value; APP: 3D vector \ 220, 230 DXF: Y and Z values of extrusion direction (optional)
	int lock_position_flag_;		// 280 Lock position flag. Locks the position of the attribute within the block reference

	string subclass_marker3_;		// 100 Subclass marker (AcDbXrecord)
	int dublicate_record_cloning_;	// 280 Duplicate record cloning flag (determines how to merge duplicate entries): 1 = Keep existing
	int multiline_attribute_;		// 2 = multiline attribute
	int constant_multiline_def_;	// 4 = constant multiline attribute definition
	int is_really_locked_;			// 70 isReallyLocked flag: 0 = unlocked /1 = locked
	int number_of_secondary_attr_; 	//70 Number of secondary attributes or attribute definitions
	int hard_pointer_of_secondary_attr_;	// 40 Hard-pointer id of secondary attribute(s) or attribute definition(s)
	Point_ alignment_point_;		// 10 Alignment point of attribute or attribute definition
									// DXF: X value; APP: 3D point
									// 20,30 DXF: Y and Z values of insertion point
	double current_annotation_scale_; // 40 current annotation scale
	string attribute_tag_string;	//  2 attribute or attribute definition tag string
	string entity_type_ = "MTEXT";	// 0 Entity type (MTEXT)
	string subclass_marker_ = "AcDbEntity";	// 100 Subclass marker (AcDbEntity)
	bool	is_in_modelspace_ = false;	// 67 Absent or zero indicates entity is in model space. 1 indicates entity is in paper space (optional)
	string layer_name_;				// 8 Layer name
	string subclass_marker_3_ = "AcDbMText"; // 100 Subclass marker (AcDbMText)

	string subclass_marker_4__;          	//      100   | Subclass_marker (AcDbText)
	double thickness_ = 0.0;            //      39    | Thickness (optional; default = 0)
	Point_ text_start_point_;         	//      10    | Text_start_point (in OCS)
										//      20, 30| Text_start_point (in OCS)
	double text_height_;              	//      40    | Text_height
	string default_value_;				//      1     | Default_value (string)
	string attr_subclass_marker_; 		//     100   | Subclass_marker (AcDbAttribute)
	string attribute_tag_;				//      2     | Attribute_tag (string; cannot contain spaces)
	int attribute_flags_;				//      70    | Attribute_flags:
	                          	  	  	  //            |     1 = Attribute_is_invisible_(does_not_appear)
	                          	  	  	  //            |     2 = This_is_a_constant_attribute
	                 	  	  	  //            |     4 = Verification_is_required_on_input_of_this_attribute
	                         	 	 	  //            |     8 = Attribute_is_preset_(no_prompt_during_insertion)
	 int field_length_ = 0;             //      73    | Field_length (optional; default = 0) (not_currently_used)
	 double text_rotation_ = 0.0;           //      50    | Text_rotation (optional; default = 0)
	 double relative_X_scale_factor_ = 1.0;   //      41    | Relative_X_scale_factor_(width)_(optional;_default_=_1)...
	 double oblique_angle_ = 0.0;             //      51    | Oblique_angle (optional; default = 0)
	 string text_style_name_ = "STANDARD";           //     7     | Text_style_name (optional; default = STANDARD)
	 int text_generation_flags_ =  0;   //     71    | Text_generation_flags (optional; default = 0). See_TEX...
	 int horizontal_text_justification_type = 0; //     72    | Horizontal_text_justification_type (optional; default...
	 int vertical_text_justification_type = 0; //     74    | Vertical_text_justification_type (optional; default =...
	 int alignment_point_x =0;           //           | Alignment_point_(in_OCS)_(optional)
	 int alignment_point_y = 0;           //     210   | Alignment_point_(in_OCS)_(optional)
	 int alignment_point_z = 0;           //     220, 230| Alignment_point_(in_OCS)_(optional)
	 int lock_position_flag = 0;       //     280   | Lock_position_flag._Locks_the_position_of_the_attribut...
	 string subclass_marker_5__;           //     100   | Subclass_marker (AcDbXrecord)
	 int duplicate_record_cloning = 0; //     280   | Duplicate_record_cloning_flag (determines_how_to_merge...
	 int MText_flag  = 0;              //     70    | MText_flag:
	                           //           |     2 = multiline_attribute
	                           //           |     4 = constant_multiline_attribute_definition
	 int isReallyLocked_flag = 0;      //     70    | isReallyLocked_flag:
	                           //           |     1 = locked
	 int number_of_secondary_attri; //     70    | Number_of_secondary_attributes_or_attribute_definitions
	 int hard_pointer_id_of_second; //     340   | Hard-pointer_id_of_secondary_attribute(s)_or_attribute...
	 int alignment_point_of_attrib; //     10    | Alignment_point_of_attribute_or_attribute_definition
	                          //    20, 30| Alignment_point_of_attribute_or_attribute_definition
	 double current_annotation_scale; //     40    | current_annotation_scale
	 string attribute_or_attribute_def; //      2     | attribute_or_attribute_definition_tag_string
	 int entity_type;               //      0     | Entity_type_(MTEXT)
	 string subclass_marker;           //      100   | Subclass_marker (AcDbEntity)
	 int absent_or_zero_indicates = 0; //     67    | Absent_or_zero_indicates_entity_is_in_model_space._1_i...

	/*
Attrib group codes
Group code Description
ATTRIB | 73Attrib group codes
Group code Description




10 Insertion point
DXF: X value; APP: 3D point
20,30 DXF: Y and Z values of insertion point
40 Nominal (initial) text height
74 | Chapter 6 ENTITIES SectionAttrib group codes
Group code Description
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
ATTRIB | 75Attrib group codes
Group code Description
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
63 Background fill color (optional):
Color to use for background fill when group code 90 is 1.
441 Transparency of background fill color (not implemented)
If group 72 and/or 74 values are nonzero then the text insertion point values
are ignored, and new values are calculated by AutoCAD based on the text
alignment point and the length of the text string itself (after applying the text
style). If the 72 and 74 values are zero or missing, then the text alignment
point is ignored and recalculated based on the text insertion point and the
length of the text string itself (after applying the text style).
	 */
protected:
	 virtual void calc_bounding_box();

public:
	Attrib(const vector<Group> &properties);
	virtual ~Attrib();


	virtual string to_json();
	virtual void to_svg(pugi::xml_node& svg_node);
};

} /* namespace dxf */

#endif /* ENTITIES_ATTRIB_H_ */
