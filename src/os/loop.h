/**
 * The Code in the 'os'-Folder derives from the SimpleWatch Example by Lewis He, see
 * https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/tree/master/examples/LVGL/SimpleWatch
 * @author Lewis He
 * @author Ivo Blöchliger
 */


void os_loop();

typedef void loop_cb_t(void);

/**
 * Stops all normal loop tasks (like interrupt handling and GUI)
 * and exclusively calls this callback
 */
void register_exclusive_loop(loop_cb_t* loop_cb);
/**
 * Resumes normal loop tasks.
 * @param loop_cb Must match the callback from the register_exclusive_loop call
 */
void release_exclusive_loop(loop_cb_t* loop_cb);
