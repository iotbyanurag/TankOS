#!/bin/bash
source common.sh

function update_leds() {
    STR=`twi_call tank_io_button_status io`
    test $? == "0" || { echo "$STR"; exit 1; }
    # STR='Buttons: status AA, pressed CC, released BB'
    STATUSES=$(sed 's/Buttons: status \(..\), pressed \(..\), released \(..\)/\1\2\3/' <<< "$STR")
    while true; do
        read -n 2 btn_status
        read -n 2 btn_pressed
        read -n 2 btn_released
        break;
    done <<< $STATUSES
    test "$btn_pressed" != "00" && echo "Pressed: $btn_pressed"
    test "$btn_released" != "00" && echo "Released: $btn_released"
    sleep 0.03
    twi_call tank_io_set_leds io -p 07 -p 00 -p $btn_status -p 00 &> /dev/null
}

while true; do update_leds; sleep 0.05; done

