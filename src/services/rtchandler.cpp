/**
 * Handle timedrift
 * 
 * when syncing by ntp, record time of sync and deviation
 *   possibly compute timedrift
 *   read and save data to spiffs
 * 
 * call every minute or so to gather statistics about temperature and on/off state
 *   possibly save data to spiffs
 * 
 * function to get current time, including drift correction, to be used instead of regular time functions
 */

