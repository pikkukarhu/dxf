/*
 * Variable.h
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <string.h>
#include <iostream>

using std::string;

namespace dxf {

class Variable {
private:
	string name;
	union U {
		int i;
		double d;
		char* s = nullptr;

		~U() { if (s != nullptr) {delete[] s; }}
	};
	U value;

public:
	inline Variable(string name, int val) 	 { this->name = name; this->value.i = val; }
	inline Variable(string name, double val) { this->name = name; this->value.d = val; }
	inline Variable(string name, char* val) { this->name = name; this->value.s = new char[strlen(val)]; strcpy(this->value.s, val); }

	inline int getInteger() 	{ return this->value.i; }
	inline double getDouble()	{ return this->value.d; }
	inline string getString()	{ return this->value.s;	}

	inline string getName() 	{ return this->name; }

	virtual ~Variable() {};
};

} /* namespace dxf */

#endif /* VARIABLE_H_ */
