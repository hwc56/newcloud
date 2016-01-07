wpa_cli -iwlan0 set_network 4 ssid '"fly"'
wpa_cli -iwlan0 set_network 4 psk '"yfa12345!"'
wpa_cli -iwlan0 enable_network 4
wpa_cli -iwlan0 select_network 4
