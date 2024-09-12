# ESP32wifiProbeReqestSniffer
ESP32-ProbeRequest-Sniffer

Compiled with Arduino IDE 2.3.2

This Arduino program for the ESP32 passively scans WiFi probe requests from clients and displays the SSIDs being sought. Additionally, the MAC address of the requesting device and the channel are displayed. Empty SSIDs are ignored.

Example Output:
Probe Request für SSID: H2_5GHz | Von MAC: C2:B5:D4:0A:7D:9A | Kanal: 10
Probe Request für SSID: TP-LINK_2.4GHz_AA4A0D | Von MAC: A6:C8:CA:DF:B2:A5 | Kanal: 7
Probe Request für SSID: test123 | Von MAC: A6:C8:CA:DF:B2:A5 | Kanal: 7
Probe Request für SSID: TP-LINK_2.4GHz_AA4A0D | Von MAC: A6:C8:CA:DF:B2:A5 | Kanal: 7
Probe Request für SSID: test123 | Von MAC: A6:C8:CA:DF:B2:A5 | Kanal: 7
Probe Request für SSID: test123 | Von MAC: A6:C8:CA:DF:B2:A5 | Kanal: 8
Probe Request für SSID: test123 | Von MAC: A6:C8:CA:DF:B2:A5 | Kanal: 8
Probe Request für SSID: TP-LINK_2.4GHz_AA4A0D | Von MAC: A6:C8:CA:DF:B2:A5 | Kanal: 8
Probe Request für SSID: test123 | Von MAC: A6:C8:CA:DF:B2:A5 | Kanal: 8
