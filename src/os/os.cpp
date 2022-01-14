/**
 * The Code in the 'os'-Folder derives from the SimpleWatch Example by Lewis He, see
 * https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/tree/master/examples/LVGL/SimpleWatch
 * @author Lewis He
 * @author Ivo Blöchliger
 */

#include "os.h"


QueueHandle_t g_event_queue_handle = NULL;
EventGroupHandle_t g_event_group = NULL;
bool lenergy = false;
bool spiffs_initialized = false;
TTGOClass *ttgo;
