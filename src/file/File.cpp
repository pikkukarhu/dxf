/*
 * Document.cpp
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#include "File.h"

#include <stdlib.h>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>
#include <iconv.h>
#include <errno.h>
#include <string.h>


using std::cout;
using std::endl;
using std::ifstream;
using std::cerr;
using std::vector;
/*
int main(int argc, char** argv) {

	if (argc < 2) {
		cerr << "Usage: dxfile file_name" << endl;
	}
	dxf::File d(argv[1]);
	d.readFile();
}
*/
namespace dxf {

File::File(string fileName) {

	this->is_ =  new ifstream(fileName);

    if (!this->is_->is_open()) {
    	// TODO create special exception class to enable advanced handling.
    	throw string("Can not open file " + fileName);
    }
}

File::~File() {
	if (this->cd_ != (iconv_t)-1) {
		iconv_close(this->cd_);
	}
	this->is_->close();
	delete this->is_;
}

void File::setCodepage(const string& codepage) {
	if (this->current_codepage_ == codepage) return;

	if (this->cd_ != (iconv_t)-1) {
		iconv_close(this->cd_);
		this->cd_ = (iconv_t)-1;
	}

	this->current_codepage_ = codepage;
	
	string iconv_name = codepage;
	if (codepage == "ANSI_1252") iconv_name = "CP1252";
	else if (codepage == "ANSI_1251") iconv_name = "CP1251";
	else if (codepage == "ANSI_1250") iconv_name = "CP1250";
	// Add more mappings as needed

	if (iconv_name != "UTF-8") {
		this->cd_ = iconv_open("UTF-8", iconv_name.c_str());
		if (this->cd_ == (iconv_t)-1) {
			cerr << "Warning: Could not open iconv for codepage " << codepage << " (" << iconv_name << ")" << endl;
		}
	}
}

string File::toUtf8(const string& input) {
	if (this->cd_ == (iconv_t)-1 || input.empty()) {
		return input;
	}

	size_t in_bytes = input.length();
	size_t out_bytes = in_bytes * 4; // UTF-8 can take up to 4 bytes per char
	string output;
	output.resize(out_bytes);

	char* in_ptr = const_cast<char*>(input.c_str());
	char* out_ptr = &output[0];

	// Reset iconv state for new conversion
	iconv(this->cd_, nullptr, nullptr, nullptr, nullptr);

	if (iconv(this->cd_, &in_ptr, &in_bytes, &out_ptr, &out_bytes) == (size_t)-1) {
		// If conversion fails, return original input as fallback
		return input;
	}

	output.resize(output.length() - out_bytes);
	return output;
}

std::istream& safeGetline(std::istream& is, std::string& t)     {
    t.clear();

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();

        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char) c;
        }
    }
    is.setstate(std::ios::eofbit);
    return is;
}



std::istream& File::readGroup(std::istream& is, Group& g) {

	string s;
	if (! safeGetline(is, s) || s == "") {
		return is;
	}
	try {
		g.groupcode = stoi(s);
	}
	catch (...) {
		g.groupcode = -15;
		cerr << "Error convert group code [" << s << "] to integer!" << endl;
	}
	safeGetline(is, g.value);
	g.value = toUtf8(g.value);
	if (g.value == "EOF") {
		is.setstate(std::ios::eofbit);
	}

	return is;
}

bool File::readGroup(Group& g) {
	string s;
	if (! safeGetline(*this->is_, s) || s == "") {
		return false;
	}
	try {
		g.groupcode = stoi(s);
	}
	catch (...) {
		g.groupcode = -15;
		cerr << "Error convert group code [" << s << "] to integer!" << endl;
	}
	safeGetline(*this->is_, g.value);
	g.value = toUtf8(g.value);
	if (g.value == "EOF") {
		this->is_->setstate(std::ios::eofbit);
		return false;
	}
	return true;
}

/*
 * Read all lines of document
 */
int File::readFile() {

    string l;

    // setlocale(6, "us_ascii");

    bool sectionStart = false;
    string section;
    vector<Group> entity;

    Group g;
    while (readGroup(*(this->is_), g)) {

    	// Start new section
    	if (g.groupcode == 0 && g.value == "SECTION") {
    		sectionStart = true;
    		section = "";
    		continue;
    	}

    	if (g.groupcode == 0 && g.value == "ENDSEC") {
    		section = "";
    		cout << "] ENDSEC" << endl;
    	    continue;
    	}

    	if (g.groupcode == 2 && sectionStart) {
    		sectionStart = false;
    		cout << g.value << " [" << endl;
    		section = g.value;
    		continue;
    	}

    	// Read entities, later for other sections also
    	if (section == "ENTITIES") {

    		if (g.groupcode == 0) { // start new entity

    			// if previous entity > 0, print entity
    			if (entity.size() > 0) {
    				bool first = true;
    				cout << "  {";
    				for (const auto &a : entity) {
    					cout << (first ? "" : ",") << "\"" << a.groupcode << "\" : \"" << a.value << "\"";
    					first = false;
    				}
    				cout << "}" << endl;
    			}
    			entity.clear();
    		}

    		entity.push_back(g);
    		continue;
    	}

    }
	cout << g.groupcode << " : " << g.value << endl;
    return EXIT_SUCCESS;
};

} /* namespace dxf */
