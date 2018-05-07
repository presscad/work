#pragma once
#include "tinyxml2.h"

bool doNcData(BUFFER_OBJ* bobj);

bool doNcData(tinyxml2::XMLDocument& doc);

void doNcResponse(BUFFER_OBJ* bobj);
