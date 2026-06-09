#include <vector>
#include "entities/TextBase.h"

namespace dxf {

class AttDef final : public TextBase {
private:
	string tag_;
	string prompt_;
	unsigned int flags_ = 0;
	unsigned int field_length_ = 0;
	unsigned int vertical_justification_ = 0;
	unsigned int horizontal_justification_ = 0;

protected:
	virtual void calc_bounding_box() override;

public:
	AttDef(const vector<Group> &properties);
	virtual ~AttDef() {}

	virtual string to_json() override;
	virtual void to_svg(pugi::xml_node& svg_node) override;
};

} /* namespace dxf */
