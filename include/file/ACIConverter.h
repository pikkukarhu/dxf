/*
 * ACIConverter.h
 *
 *  Created on: May 24, 2026
 *      Author: Gemini CLI
 */

#ifndef FILE_ACICONVERTER_H_
#define FILE_ACICONVERTER_H_

#include <cstdint>
#include <array>

namespace dxf {

struct RGB;

class ACIConverter {
public:
    static RGB aciToRgb(int index);

private:
    static const uint32_t ACI_TABLE[256];
};

} /* namespace dxf */

#endif /* FILE_ACICONVERTER_H_ */
