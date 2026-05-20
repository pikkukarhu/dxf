/*
 * Text.cpp
 *
 *  Created on: 3 Dec 2024
 *      Author: hobbes
 */

#include "entities/Text.h"

namespace dxf {

Text::Text(const vector<Group> &properties) : Entity(properties) {
    // Set default values for optional attributes
    this->thickness_ = 0.0;          // Group code 39, default = 0
    this->text_height_ = 1.0;        // Group code 40, required
    this->text_rotation_ = 0.0;           // Group code 50, default = 0
    this->x_scale_factor_ = 1.0;   // Group code 41, default = 1
    this->oblique_angle_ = 0.0;      // Group code 51, default = 0
    this->text_style_name_ = "STANDARD"; // Group code 7, default = "STANDARD"
    this->text_generation_flags_ = 0;    // Group code 71, default = 0
    this->horizontal_justification_ = 0; // Group code 72, default = 0
    this->vertical_justification_ = 0;   // Group code 73, default = 0

    this->extrusion_direction_.x_ = 0.0; // Group codes 210, 220, 230
    this->extrusion_direction_.y_ = 0.0;
    this->extrusion_direction_.z_ = 1.0;

    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 10: // First alignment point (X value)
                this->first_alignment_point_.x_ = stod(g.value);
                break;
            case 20: // First alignment point (Y value)
                this->first_alignment_point_.y_ = stod(g.value);
                break;
            case 30: // First alignment point (Z value)
                this->first_alignment_point_.z_ = stod(g.value);
                break;
            case 11: // Second alignment point (X value)
                this->second_alignment_point_.x_ = stod(g.value);
                break;
            case 21: // Second alignment point (Y value)
                this->second_alignment_point_.y_ = stod(g.value);
                break;
            case 31: // Second alignment point (Z value)
                this->second_alignment_point_.z_ = stod(g.value);
                break;
            case 39: // Thickness (optional)
                this->thickness_ = stod(g.value);
                break;
            case 40: // Text height
                this->text_height_ = stod(g.value);
                break;
            case 1: // Default text value (string)
                this->default_value_ = g.value;
                break;
            case 50: // Text rotation
                this->text_rotation_ = stod(g.value);
                break;
            case 41: // Relative X scale factor
                this->x_scale_factor_ = stod(g.value);
                break;
            case 51: // Oblique angle
                this->oblique_angle_ = stod(g.value);
                break;
            case 7: // Text style name
                this->text_style_name_ = g.value;
                break;
            case 71: // Text generation flags
                this->text_generation_flags_ = stoi(g.value);
                break;
            case 72: // Horizontal justification
                this->horizontal_justification_ = stoi(g.value);
                break;
            case 73: // Vertical justification
                this->vertical_justification_ = stoi(g.value);
                break;
            case 210: // Extrusion direction (X value)
                this->extrusion_direction_.x_ = stod(g.value);
                break;
            case 220: // Extrusion direction (Y value)
                this->extrusion_direction_.y_ = stod(g.value);
                break;
            case 230: // Extrusion direction (Z value)
                this->extrusion_direction_.z_ = stod(g.value);
                break;
            default:
                // Ignore unrecognized group codes
                break;
        }
    }
}


Text::~Text() {
	// TODO Auto-generated destructor stub
}

} /* namespace dxf */
