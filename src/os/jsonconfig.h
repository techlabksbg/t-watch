/**
 * @author Ivo Blöchliger
 */
#pragma once

#include "os.h"

void loadJsonConfig();
void saveJsonConfig();

extern DynamicJsonDocument* configJson;
