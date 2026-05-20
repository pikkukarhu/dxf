/*
 * MText.cpp
 *
 *  Created on: 21 Apr 2022
 *      Author: hobbes
 */

#include "entities/MText.h"
#include <cmath>
#include <string>

namespace dxf {

MText::MText(const vector<Group> &properties) : Entity(properties) {
    // Set default values for optional attributes
	this->text_style_name_ = "STANDARD";
    this->nominal_text_height_ = 1.0;           // Group code 40
    this->reference_rectangle_width_ = 0.0;     // Group code 41
    this->attachment_point_ = AttachmentPoints::BottomLeft;
    this->drawing_direction_ = DrawingDirection::LeftToRight;
    this->mtext_line_spacing_style_ = MtextLineSpacingStyle::AtLeast;
    this->mtext_line_spacing_factor_ = 1.0;    	// Group code 44, default = 1
    this->background_fill_setting_ = BackgroundFillSetting::BackgroundFillOff;

    this->extrusion_direction_.x_ = 0.0; 		// Group codes 210, 220, 230
    this->extrusion_direction_.y_ = 0.0;
    this->extrusion_direction_.z_ = 1.0;

    this->x_axis_direction_vector_.x_ = 1.0;   // Group code 11, default  x-axis direction.
    this->x_axis_direction_vector_.y_ = 0.0;
    this->x_axis_direction_vector_.z_ = 0.0;


	std::string additional_text = "";
    for (const auto &g : properties) {
    	double rotation;
        switch (g.groupcode) {
        	case 10:    // insertion point
        		this->insertion_point_.x_ = stod(g.value);
        		break;
        	case 20:
        		this->insertion_point_.y_ = stod(g.value);
        		break;
        	case 30:
        		this->insertion_point_.z_ = stod(g.value);
        		break;
            case 40:
                this->nominal_text_height_ = stod(g.value);
                break;
                /*
            case 41:
                this->reference_rectangle_width_ = stod(g.value);
                break;
                */
            case 71:
            	this->attachment_point_ = static_cast<AttachmentPoints>(stoi(g.value));
            	break;
            case 72:
            	this->drawing_direction_ = static_cast<DrawingDirection>(stoi(g.value));
            	break;
            case 3:
            	additional_text += g.value;
            	break;
            case 1:
            	this->text_ = additional_text + g.value;
            	break;
            case 7:
            	this->text_style_name_ = g.value;
            	break;
            case 210:
            	this->extrusion_direction_.x_ = stod(g.value);
            	break;
            case 220:
            	this->extrusion_direction_.y_ = stod(g.value);
            	break;
            case 230:
            	this->extrusion_direction_.z_ = stod(g.value);
            	break;
            case 11:
            	this->x_axis_direction_vector_.x_ = stod(g.value);
            	break;
            case 21:
            	this->x_axis_direction_vector_.y_ = stod(g.value);
            	break;
            case 31:
            	this->x_axis_direction_vector_.z_ = stod(g.value);
            	break;
            	/*
            case 41:
            	this->max_horizontal_width_ = stod(g.value);
            	break;
            	*/
            case 43:
            	this->vertical_height_of_mtext_entity_ = stod(g.value);
            	break;
            case 44:
            	this->mtext_line_spacing_factor_ = stod(g.value);
            	break;
            case 45:
            	this->fill_box_scale_ = stod(g.value);
            	break;
            case 48:
            	this->column_width_ = stod(g.value);
            	break;
            case 49:
            	this->column_gutter_ = stod(g.value);
            	break;
            case 50:
            	rotation = stod(g.value);
            	this->x_axis_direction_vector_.x_ = cos(rotation);
            	this->x_axis_direction_vector_.y_ = sin(rotation);
            	this->x_axis_direction_vector_.z_ = 0.0;
            	break;
            case 63:
            	this->background_color_ = stoi(g.value);
            	break;
            case 73:
            	this->mtext_line_spacing_style_ = static_cast<MtextLineSpacingStyle>(stoi(g.value));
            	break;
            case 75:
            	this->column_type_ = stoi(g.value);
            	break;
            case 76:
            	this->column_count_ = stoi(g.value);
            	break;
            case 78:
            	this->column_flow_reversed_ = stoi(g.value) != 0;
            	break;
            case 79:
            	this->column_autoheight_ = stoi(g.value) != 0;
            	break;
            case 90:
            	this->background_fill_setting_ = static_cast<BackgroundFillSetting>(stoi(g.value));
            	break;
            case 420:
            	this->background_color_rgb_[0] = to_rgb(stoi(g.value));
            	break;
            case 421:
            	this->background_color_rgb_[1] = to_rgb(stoi(g.value));
            	break;
            case 422:
            	this->background_color_rgb_[2] = to_rgb(stoi(g.value));
            	break;
            case 423:
            	this->background_color_rgb_[3] = to_rgb(stoi(g.value));
            	break;
            case 424:
            	this->background_color_rgb_[4] = to_rgb(stoi(g.value));
            	break;
            case 425:
            	this->background_color_rgb_[5] = to_rgb(stoi(g.value));
            	break;
            case 426:
            	this->background_color_rgb_[6] = to_rgb(stoi(g.value));
            	break;
            case 427:
            	this->background_color_rgb_[7] = to_rgb(stoi(g.value));
            	break;
            case 428:
            	this->background_color_rgb_[8] = to_rgb(stoi(g.value));
            	break;
            case 429:
            	this->background_color_rgb_[9] = to_rgb(stoi(g.value));
            	break;
            case 430:
            	this->background_color_name_[0] = g.value;
            	break;
            case 431:
            	this->background_color_name_[1] = g.value;
            	break;
            case 432:
            	this->background_color_name_[2] = g.value;
            	break;
            case 433:
            	this->background_color_name_[3] = g.value;
            	break;
            case 434:
            	this->background_color_name_[4] = g.value;
            	break;
            case 435:
            	this->background_color_name_[5] = g.value;
            	break;
            case 436:
            	this->background_color_name_[6] = g.value;
            	break;
            case 437:
            	this->background_color_name_[7] = g.value;
            	break;
            case 438:
            	this->background_color_name_[8] = g.value;
            	break;
            case 439:
            	this->background_color_name_[9] = g.value;
            	break;
            case 441:
            	this->transparency_of_background_ = stod(g.value);
            	break;
            	/*
            case 45:
            	this->background_fill_color_ = stoi(g.value); // 63 Background fill color (optional):
            	break;*/
            case 41:
            	this->max_horizontal_width_ = stod(g.value);
            	break;
            case 42:
            	this->horizontal_width_of_the_characters_ = stod(g.value);
            	break;
        }
    }
    calc_bounding_box();
}


MText::~MText() {
	// TODO Auto-generated destructor stub
}

} /* namespace dxf */
