//
//  NovationLaunchpadController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/29/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "NovationLaunchpadController.hpp"

void WriteNovationPadColor(const uint32_t pad, const uint32_t color) {
  NovationLaunchpadController::GetInstance().SendColorMessage(pad, color);
}
