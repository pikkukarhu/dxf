/*
 * TextBase.cpp
 *
 *  Created on: May 23, 2026
 *      Author: Gemini CLI
 */

#include "entities/TextBase.h"
#include "tables/Tables.h"
#include "tables/Table.h"
#include "entries/Style.h"
#include <algorithm>

namespace dxf {

static string mapFont(const string& fontFile) {
    string lowerFont = fontFile;
    std::transform(lowerFont.begin(), lowerFont.end(), lowerFont.begin(), 
                   [](unsigned char c){ return std::tolower(c); });
    
    // Remove extension if present for easier matching
    size_t lastdot = lowerFont.find_last_of(".");
    string baseName = (lastdot == string::npos) ? lowerFont : lowerFont.substr(0, lastdot);

    // standard.lff, romans.lff, simplex.shx, txt.shx > Arial or Liberation Sans
    if (baseName == "standard" || baseName == "romans" || 
        baseName == "simplex" || baseName == "txt") {
        return "Arial, 'Liberation Sans', sans-serif";
    }
    
    // iso3098.lff, iso3098.shx > Bahnschrift or Roboto
    if (baseName == "iso3098") {
        return "Bahnschrift, Roboto, sans-serif";
    }
    
    // romanp.lff, romanc.shx > Times New Roman
    if (baseName == "romanp" || baseName == "romanc") {
        return "'Times New Roman', serif";
    }
    
    return "Arial, sans-serif";
}

TextBase::TextBase(const vector<Group> &properties) : Entity(properties) {
    for (const auto &g : properties) {
        switch (g.groupcode) {
            case 1:
                this->value_ = g.value;
                break;
            case 7:
                this->style_name_ = g.value;
                break;
            case 10:
                this->insertion_point_.x_ = std::stod(g.value);
                break;
            case 20:
                this->insertion_point_.y_ = std::stod(g.value);
                break;
            case 30:
                this->insertion_point_.z_ = std::stod(g.value);
                break;
            case 40:
                this->height_ = std::stod(g.value);
                break;
            case 210:
                this->extrusion_direction_.x_ = std::stod(g.value);
                break;
            case 220:
                this->extrusion_direction_.y_ = std::stod(g.value);
                break;
            case 230:
                this->extrusion_direction_.z_ = std::stod(g.value);
                break;
        }
    }
}

void TextBase::resolve(const Tables& tables, bool isBlackBackground) {
    Entity::resolve(tables, isBlackBackground);

    Table* styleTable = tables.getTable("STYLE");
    if (styleTable != nullptr) {
        Style* style = dynamic_cast<Style*>(styleTable->getEntry(this->style_name_));
        if (style != nullptr) {
            this->resolved_font_family_ = mapFont(style->getPrimaryFontFile());
        } else {
            // Fallback if style not found
            this->resolved_font_family_ = "Arial, sans-serif";
        }
    }
}

string TextBase::to_string() {
    string s = Entity::to_string();
    s += ", \"value\" : \"" + value_ + "\"";
    s += ", \"style_name\" : \"" + style_name_ + "\"";
    s += ", \"insertion_point\" : " + insertion_point_.as_string();
    s += ", \"height\" : " + std::to_string(height_);
    s += ", \"rotation\" : " + std::to_string(rotation_);
    s += ", \"resolved_font_family\" : \"" + resolved_font_family_ + "\"";
    s += ", \"extrusion_direction\" : " + extrusion_direction_.as_string();
    return s;
}

void TextBase::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	Entity::write_to_json_writer(writer);
	writer.Key("value");               writer.String(this->value_.c_str());
	writer.Key("style_name");          writer.String(this->style_name_.c_str());
    writer.Key("resolved_font_family"); writer.String(this->resolved_font_family_.c_str());
	writer.Key("insertion_point");     this->insertion_point_.write_to_json_writer(writer);
	writer.Key("height");              writer.Double(this->height_);
	writer.Key("rotation");            writer.Double(this->rotation_);
	writer.Key("extrusion_direction"); this->extrusion_direction_.write_to_json_writer(writer);
}

} /* namespace dxf */
