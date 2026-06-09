#include <pugixml.hpp>
#include "entities/TextBase.h"

namespace dxf {

class Text final : public TextBase {
private:
	double thickness_ = 0.0; // 39
	Point_ second_alignment_point_; // 11, 21, 31
	int horizontal_justification_ = 0; // 72
	int vertical_justification_ = 0; // 73
	int text_generation_flags_ = 0; // 71
	double x_scale_factor_ = 1.0; // 41
	double oblique_angle_ = 0.0; // 51

protected:
	virtual void calc_bounding_box() override;

public:
	Text(const vector<Group> &properties);
	virtual ~Text() {}

	virtual string to_json() override;
	virtual void to_svg(pugi::xml_node& svg_node) override;
};

} /* namespace dxf */
