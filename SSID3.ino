#include <WiFi.h>
#include "esp_wifi.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  // WiFi initialisieren
  WiFi.mode(WIFI_MODE_STA);

  // Zusätzliche Initialisierung des WiFi-Stacks
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);

  // Promiscuous Mode Filter setzen
  wifi_promiscuous_filter_t filt = {
    .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT
  };
  esp_wifi_set_promiscuous_filter(&filt);

  // Promiscuous Mode aktivieren
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_callback);

  // Starten wir mit Kanal 1
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
}

void loop() {
  static unsigned long last_channel_change = 0;
  static uint8_t current_channel = 1;
  unsigned long now = millis();

  // Wechsel des Kanals alle 500 ms (Anzeige des Kanalwechsels entfernt)
  if (now - last_channel_change > 500) {
    last_channel_change = now;
    current_channel++;
    if (current_channel > 13) current_channel = 1;
    esp_wifi_set_channel(current_channel, WIFI_SECOND_CHAN_NONE);
  }
}

// Definitionen für WiFi-Pakettypen
#define WIFI_PKT_MGMT   0x00
#define WIFI_PKT_CTRL   0x04
#define WIFI_PKT_DATA   0x08
#define WIFI_PKT_MISC   0x0C

// Subtypen für Management-Pakete
#define WIFI_MGMT_PROBE_REQ 0x04

// Callback-Funktion für empfangene Pakete
void promiscuous_rx_callback(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) {
    // Nur Management-Pakete interessieren uns
    return;
  }

  wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t*)buf;
  uint8_t* payload = pkt->payload;
  uint16_t len = pkt->rx_ctrl.sig_len;

  // Überprüfen, ob es ein Probe Request ist
  uint8_t frame_control = payload[0];
  uint8_t subtype = (frame_control & 0xF0) >> 4;

  if (subtype != WIFI_MGMT_PROBE_REQ) {
    // Kein Probe Request
    return;
  }

  // SSID aus dem Probe Request extrahieren
  uint8_t ssid_len = 0;
  char ssid[33]; // Maximale SSID-Länge ist 32 Zeichen

  // Management-Frame hat 24 Bytes Fixed Fields
  int pos = 24; // Beginn der Tagged Parameters

  // Extrahiere die Source MAC Adresse (Adresse 2)
  uint8_t sa[6]; // Source Address
  memcpy(sa, &payload[10], 6); // Adresse 2 beginnt bei Byte 10

  // Extrahiere den Kanal, auf dem das Paket empfangen wurde
  uint8_t channel = pkt->rx_ctrl.channel;

  // Initialisiere eine Variable, um festzustellen, ob eine SSID gefunden wurde
  bool ssid_found = false;

  while (pos < len) {
    uint8_t tag_number = payload[pos];
    uint8_t tag_length = payload[pos + 1];

    if (tag_number == 0) { // SSID Parameter Set
      ssid_len = tag_length;
      if (ssid_len > 32) ssid_len = 32;
      memcpy(ssid, &payload[pos + 2], ssid_len);
      ssid[ssid_len] = '\0';

      // Prüfen, ob die SSID nicht leer ist
      if (ssid_len > 0) {
        ssid_found = true;
      }

      break; // Wir haben die SSID gefunden, Schleife beenden
    }
    pos += 2 + tag_length;
  }

  // Nur ausgeben, wenn eine nicht leere SSID gefunden wurde
  if (ssid_found) {
    // Ausgabe der SSID, MAC-Adresse und Kanal
    Serial.print("Probe Request für SSID: ");
    Serial.print(ssid);
    Serial.print(" | Von MAC: ");

    // MAC-Adresse ausgeben
    for (int i = 0; i < 6; i++) {
      if (sa[i] < 16) Serial.print("0");
      Serial.print(sa[i], HEX);
      if (i < 5) Serial.print(":");
    }

    Serial.print(" | Kanal: ");
    Serial.println(channel);
  }
}
