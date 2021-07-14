/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../os/os.h"
#include "../apps/app.h"

/**
 * Call this function in the show-method of your App.
 * It will return true, when the file exists on SPIFFS.
 * Otherwise, it will activate the WiFi-Connection if necessary,
 * then download the file. 
 * @param app Pointer to the app (normally this in the show-method of your App)
 * @param url The URL of the file to be downloaded
 * @param filename The filename (including a leading slash '/')
 * @param remove Delete the file first, to force the download of the file.
 */
bool downloadToSPIFFS(App* app, const char * url, const char * filename, bool remove=false);
