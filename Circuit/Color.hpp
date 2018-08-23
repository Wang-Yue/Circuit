//
//  Color.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Color_hpp
#define Color_hpp

#include "TypeDefs.hpp"

class Color {
public:
  Color(): Color(0) {
  }
  Color(const ColorBrightness &red,
        const ColorBrightness &green,
        const ColorBrightness &blue) {
    _code = red;
    _code <<= 8;
    _code |= green;
    _code <<= 8;
    _code |= blue;
  }
  
  Color(const ColorCode &code) : _code(code) {
  }

  Color(const Color &color): _code(color._code) {
  }
  
  ColorCode GetColorCode() {
    return _code;
  }
private:
  ColorCode _code;

};

#endif /* Color_hpp */
