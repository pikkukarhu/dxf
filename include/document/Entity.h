/*
 * Entity.h
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */
#pragma once

#ifndef DOCUMENT_ENTITY_H_
#define DOCUMENT_ENTITY_H_

#include <vector>
#include <string>
#include <pugixml.hpp>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "file/File.h"

using std::vector;
using std::string;

namespace dxf {

class Tables;

struct RGB {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct BoundingBox {
    double min_x;
    double min_y;
    double max_x;
    double max_y;
    bool initialized;

    inline BoundingBox() : min_x(0), min_y(0), max_x(0), max_y(0), initialized(false) {}

    inline void update(double x, double y) {
        if (!initialized) {
            min_x = max_x = x;
            min_y = max_y = y;
            initialized = true;
        } else {
            if (x < min_x) min_x = x;
            if (x > max_x) max_x = x;
            if (y < min_y) min_y = y;
            if (y > max_y) max_y = y;
        }
    }

    inline void combine(const BoundingBox& other) {
        if (!other.initialized) return;
        update(other.min_x, other.min_y);
        update(other.max_x, other.max_y);
    }

    inline double width() const { return max_x - min_x; }
    inline double height() const { return max_y - min_y; }
};

struct Rect {
	double x;
	double y;
	double width;
	double heigth;

	inline Rect() {x = y = width = heigth = 0; }

	inline Rect(double x, double y, double width, double heigth) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->heigth = heigth;
	}

    inline BoundingBox to_bounding_box() const {
        BoundingBox bb;
        bb.min_x = x;
        bb.min_y = y;
        bb.max_x = x + width;
        bb.max_y = y + heigth;
        bb.initialized = true;
        return bb;
    }
};

class Entity {
private:
	/* Should these be references? Or there,s name from file *and* reference, solved after read whole file
	 *  -- depends how much we use dxf::entitity / dxf::document in code?
	 *  -- name better, can save to Json- file or document db as-is.virtual void to_svg(pugi::xml_node& svgNode);
	 */
	string	type_;					// Entity type name. Eg LINE, POINT, CIRCLE ...
	int		id_;
	string	readable_id_;
	string	handle_;				// Entity handle
	string	line_style_;			// Linestyle name;
	string	subclass_marker_;		// Subclass marker (AcDbEntity) not omitted
	bool	model_space_ = true;	// Is entity in modelspace (false = paperspace)
	string	layer_	= "0";			// Layer name;
	string	line_type_ = "BYLAYER";	// Default 256 Byblock
	int		color_number_ = 0;		// Default 0 Bylayer
	int		line_weight_;
	double	linetype_scale_ = 1.0;
	bool	visible_	= true;		// Visiblity of entity, default = true;
	RGB		rgb_;
	int		shadow_mode_ = 3;		// 0 = Casts and receives shadows
									// 1 = Casts shadows							// 2 = Receives shadows
protected:							// 3 = Ignores shadows
	Rect bounding_box_; 			// Bounding box of element

	// Resolved properties
	int resolved_color_ = 0;
	RGB resolved_rgb_ = {0, 0, 0};
	string resolved_linetype_ = "CONTINUOUS";
	int resolved_line_weight_ = -1;

	virtual void calc_bounding_box() = 0;
	void draw_bounding_box(pugi::xml_node& svg_node);
	void add_common_svg_attributes(pugi::xml_node& node);
public:
	static bool show_bounding_box;
	static int next_entity_id;
	static inline void setShowBoundingBox(bool show) { show_bounding_box = show; }

	struct Point_ {
		double x_;
		double y_;
		double z_;

		inline std::string as_string() {
			return
				 "{\"x\" : " + std::to_string(x_) + ", "
				+ "\"y\" : " + std::to_string(y_) + ", "
				+ "\"z\" : " + std::to_string(z_) + "}"; }
		
		inline void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
			writer.StartObject();
			writer.Key("x"); writer.Double(x_);
			writer.Key("y"); writer.Double(y_);
			writer.Key("z"); writer.Double(z_);
			writer.EndObject();
		}
	};

	Entity(const std::vector<dxf::Group>&);
	virtual ~Entity();

	inline string	get_type() { return this->type_;	}
	inline int		get_id() { return this->id_; }
	inline string	get_readable_id() { return this->readable_id_; }
	inline string	get_handle() { return this->handle_;}
	inline string	get_linestyle() { return this->line_style_;}
	inline string	get_subclass_marker() { return this->subclass_marker_;}
	inline bool		is_modelspace() { return this->model_space_;}
	inline string	get_layer() { return this->layer_;}
	inline string	get_linetype() { return this->line_type_;}
	inline int		get_colorNumber() { return this->color_number_ ;}
	inline int		get_line_weight() { return this->line_weight_;}
	inline double	get_linetype_scale() { return this->linetype_scale_;}
	inline bool		is_visible() { return this->visible_;}
	inline RGB		get_RGB() { return this->rgb_;}
	inline RGB		get_resolved_RGB() { return this->resolved_rgb_; }
	inline int		get_shadow_mode() { return this->shadow_mode_;}
	inline Rect 	get_bounding_box() { return this->bounding_box_; };

	string get_svg_color();

	virtual void resolve(const Tables& tables, bool isBlackBackground = true);

	virtual string to_string();
	virtual string to_json() = 0;
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer);
	virtual void to_svg(pugi::xml_node& svgNode) = 0;
};

} /* namespace dxf */

#endif /* DOCUMENT_ENTITY_H_ */
