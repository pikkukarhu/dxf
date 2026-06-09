#include <cmath>
#include "entities/TextBase.h"

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

class MText final : public TextBase {
private:
	inline RGB to_rgb(long color) {
		int32_t c = (int32_t)color;
		RGB rgb;
		rgb.blue = 0xFF & c;
		rgb.green = (0xFF << 8) & c;
		rgb.red = (0xFF << 16) & c;

		return rgb;
	}

	double reference_rectangle_width_; // 41 Reference rectangle width
	AttachmentPoints attachment_point_; // group code 71
	DrawingDirection drawing_direction_; // group code 72
	Point_ x_axis_direction_vector_; // 11, 21, 31
	double vertical_height_of_mtext_entity_; // 43
	MtextLineSpacingStyle mtext_line_spacing_style_; // 73
	double mtext_line_spacing_factor_; // 44
	BackgroundFillSetting background_fill_setting_; // 90
	int background_color_; // 63
	RGB background_color_rgb_[10]; // 420-429
	std::string background_color_name_[10]; // 430-439
	double fill_box_scale_; // 45
	double transparency_of_background_; // 441
	int column_type_; // 75
	int column_count_; // 76
	bool column_flow_reversed_; // 78
	bool column_autoheight_ ; // 79
	double column_width_; // 48
	double column_gutter_ ; // 49
	std::vector<double> column_heights_;

protected:
	virtual void calc_bounding_box() override;

public:
	MText(const vector<Group> &properties);
	virtual ~MText() {}

	virtual string to_json() override;
	virtual void to_svg(pugi::xml_node& svg_node) override;
};

} /* namespace dxf */
