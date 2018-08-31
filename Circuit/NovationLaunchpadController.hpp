//
//  NovationLaunchpadController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/29/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef NovationLaunchpadController_hpp
#define NovationLaunchpadController_hpp

#include <cstdint>
#include <RtMidi.h>


class NovationLaunchpadController {
public:
  NovationLaunchpadController(NovationLaunchpadController const&) = delete;
  void operator=(NovationLaunchpadController const&)  = delete;
  static NovationLaunchpadController& GetInstance();
  void SendColorMessage(const char &pad, const  uint32_t &colorCode);
private:
  NovationLaunchpadController();
  RtMidiIn  *_midi_in = 0;
  RtMidiOut *_midi_out = 0;
};

#endif /* NovationLaunchpadController_hpp */
