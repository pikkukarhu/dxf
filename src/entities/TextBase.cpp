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

string TextBase::convertLffGlyphToSvgPath(const std::vector<Polyline>& glyphLines, double originX, double originY, double scale) {
    std::ostringstream pathData;
    
    for (const auto& polyline : glyphLines) {
        if (polyline.empty()) continue;
        
        // Start the subpath (M = Move To)
        // Note: Y is kept positive here because it natively matches your upward CAD Y-axis!
        pathData << "M " << (originX + polyline[0].x * scale) 
                 << " " << (originY + polyline[0].y * scale) << " ";
                 
        // Draw the line strings (L = Line To)
        for (size_t i = 1; i < polyline.size(); ++i) {
            pathData << "L " << (originX + polyline[i].x * scale) 
                     << " " << (originY + polyline[i].y * scale) << " ";
        }
    }
    
    // Wrap it in a standard SVG path node
    return "<path d=\"" + pathData.str() + "\" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />";
}

string TextBase::to_string() {
    string s = Entity::to_string();
    double correctedAngle = -rotation_;
    s += ", \"transform\" : \"scale(1, -1)\" \" \";" /*rotate(" + std::to_string(correctedAngle) + ", " + std::to_string(x) + ", " + std::to_string(-y) + ")*/;
    s += ", \"value\" : \"" + value_ + "\"";
    s += ", \"style_name\" : \"" + style_name_ + "\"";
    s += ", \"insertion_point\" : " + insertion_point_.as_string();
    s += ", \"height\" : " + std::to_string(height_);
    s += ", \"rotation\" : " + std::to_string(correctedAngle /*rotation*/);
    s += ", \"resolved_font_family\" : \"" + resolved_font_family_ + "\"";
    s += ", \"extrusion_direction\" : " + extrusion_direction_.as_string();
    return s;
}

void TextBase::to_svg(pugi::xml_node& svg_node) {
    // 1. read test character by character, decode LFF segments, and convert to SVG path
    // 2. for each character, get its glyph lines (polylines) and convert to SVG path using convertLffGlyphToSvgPath
    // 3. append the resulting SVG path to the svg_node
    // 4. apply transformations (scale, rotate, translate) to position the text correctly in the SVG
    // Note: This is a placeholder implementation. Actual LFF decoding and glyph extraction is needed
    // For demonstration, we will just create a simple text element with the value


    // Create <g> group for the text
    pugi::xml_node textGroup = svg_node.append_child("g");
    // Add extra (non vivible) attributes for original text content and style
    textGroup.append_attribute("data-value").set_value(value_.c_str());
    textGroup.append_attribute("data-style-name").set_value(style_name_.c_str());

    string s = value_;
    for (size_t i = 0; i < s.length(); ++i) {
        char c = s[i];
        // Here you would decode the LFF for character 'c' and get its glyph lines
        // For now, we will just create a placeholder path for each character
        std::vector<Polyline> glyphLines; // This should be filled with actual glyph data
        // Select right glyphLines based on character 'c' and charecter style (font name) from the font's LFF data

        // Calculate origin for this character based on insertion point and character index
        // need update insertion point to endpoint of previous character, for now use bounding box of glyphLines

        // That is: For every added character, we need to update the insertion point for the next character based on the width of the current character's glyph lines. This requires knowing the bounding box of the glyph lines.
        // For performance, every character's glyph lines should be precomputed and stored in a map for quick access with it's bounding box.

        double originX = insertion_point_.x_ + i * height_; // Simple horizontal placement
        double originY = insertion_point_.y_;
        double scale = height_; // Scale based on text height

        string pathSvg = convertLffGlyphToSvgPath(glyphLines, originX, originY, scale);
        pugi::xml_document doc;
        doc.load_string(pathSvg.c_str());
        svg_node.append_copy(doc.first_child());
    }
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
