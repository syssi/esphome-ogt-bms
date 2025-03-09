#!/bin/bash

if [[ $2 == tests/* ]]; then
  C="../components"
else
  C="components"
fi

esphome -s ble_name SmartBat-A03747 -s mac_address B4:52:A9:09:91:42 -s external_components_source $C ${1:-run} ${2:-esp32-ble-example-faker.yaml}
