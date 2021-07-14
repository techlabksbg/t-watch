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
 * @param filename The filename (including a leading slash '/')
 * @param url The URL of the file to be downloaded
 * @param root_ca The root-certificate, mandatory if downloading by https!
 * @param remove Delete the file first, to force the download of the file.
 */
bool downloadToSPIFFS(App* app, const char * filename, const char * url, const char* root_ca=nullptr, bool remove=false);

/**
 * Call this function to start screencast streaming to port 8080
 * on the given host. Call it with nullptr to terminate.
 * WiFi must be on.
 * The images are raw 240x240*2 Bytes.
 * @param host Hostname or IP address.
 */
void streamToIP(const char* host);
