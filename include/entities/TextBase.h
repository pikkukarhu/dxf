/*
 * TextBase.h
 *
 *  Created on: May 23, 2026
 *      Author: Gemini CLI
 */

#pragma once

#ifndef ENTITIES_TEXTBASE_H_
#define ENTITIES_TEXTBASE_H_

#include "document/Entity.h"

namespace dxf {

class TextBase : public Entity {
protected:
    string value_;
    string style_name_ = "STANDARD";
    Point_ insertion_point_;
    double height_ = 1.0;
    double rotation_ = 0.0;
    Point_ extrusion_direction_ = {0.0, 0.0, 1.0};

public:
    TextBase(const vector<Group> &properties);
    virtual ~TextBase() {}

    virtual string to_string() override;
};

} /* namespace dxf */

#endif /* ENTITIES_TEXTBASE_H_ */
