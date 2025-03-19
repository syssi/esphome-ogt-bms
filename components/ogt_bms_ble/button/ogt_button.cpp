#include "ogt_button.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace ogt_bms_ble {

static const char *const TAG = "ogt_bms_ble.button";

void OgtButton::dump_config() { LOG_BUTTON("", "OgtBmsBle Button", this); }
void OgtButton::press_action() { this->parent_->send_command(this->holding_register_, 2); }

}  // namespace ogt_bms_ble
}  // namespace esphome
