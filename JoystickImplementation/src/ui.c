#include <asf.h>
#include "ui.h"
#include "joystick.h"


// called every Start-Of-Frame (1 ms) when interface is enabled
void ui_process(uint16_t framenumber) {
    joystick();
}