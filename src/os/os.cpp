#include "os.h"


QueueHandle_t g_event_queue_handle = NULL;
EventGroupHandle_t g_event_group = NULL;
bool lenergy = false;
bool spiffs_initialized = false;
TTGOClass *ttgo;
