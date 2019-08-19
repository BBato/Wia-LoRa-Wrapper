/*
  Arduino Library for the Wia LoRaWAN shield

  Development environment specifics:
  Arduino IDE 1.8.8
  Microsoft Visual Studio Code
  Wia LoRaWAN module

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Made by Wia Technologies - find us at wia.io .
*/


#include <Wia_LoRaWAN_Library.h>

#include <lmic.h>
#include <hal/hal.h>

static const u1_t PROGMEM APPEUI[8] = { 0x65, 0xF7, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) {
 memcpy_P(buf, APPEUI, 8);
}

static const u1_t PROGMEM DEVEUI[8] = { 0x65, 0x7C, 0x56, 0xF6, 0xD3, 0x66, 0xC1, 0xDE };
void os_getDevEui (u1_t* buf) {
 memcpy_P(buf, DEVEUI, 8);
}

static const u1_t PROGMEM APPKEY[16] = { 0xE2, 0x68, 0x8A, 0x50, 0xF9, 0x4F, 0x31, 0xC7, 0x9B, 0xB2, 0xE8, 0xDB, 0x05, 0x01, 0xEC, 0x95};
void os_getDevKey (u1_t* buf) {
 memcpy_P(buf, APPKEY, 16);
}

const lmic_pinmap lmic_pins = {
  .nss = 32,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 12,
  .dio = {14, 26, LMIC_UNUSED_PIN},
};


void Wia_LoRaWAN::checkWakeupReason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0 :  Serial.println("Wakeup caused by external interrupt 0."); break;
    case ESP_SLEEP_WAKEUP_EXT1 :  Serial.println("Wakeup caused by external interrupt 1."); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer."); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;    
  }
}


Wia_LoRaWAN::Wia_LoRaWAN(){

  Serial.begin(115200);
  delay(200);
  Serial.println("os_init();");
  os_init();
  Serial.println("end.");

  LMIC_reset();
  LMIC_setClockError(MAX_CLOCK_ERROR * 20 / 100);
  LMIC_setDrTxpow(DR_SF7, 10);
  LMIC_selectSubBand(1);
  Serial.println("Checking wakeup reason?");
  checkWakeupReason();


  Serial.println("Finished.");

}


void Wia_LoRaWAN::onLoop(){
  os_runloop_once();
}


bool Wia_LoRaWAN::sendMessage( String packet ) {
  
  Serial.println(F("sendMessage()"));
  uint8_t outbox[packet.length()];
  
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
      Serial.println(F("Transmitter busy, try again later!"));
      return false;
  } else {
    for (int i = 0; i < packet.length(); i++) {
      outbox[i] = packet[i];
    }
    
    LMIC_setTxData2(1, outbox, sizeof(outbox), 0);
    Serial.println(F("Transmitter queued message."));
  }

  return true;
  
}

void onEvent (ev_t ev) {
  
  Serial.print(os_getTime());
  Serial.print(": ");
  
  switch (ev) {
    case EV_SCAN_TIMEOUT:
      Serial.println(F("EV_SCAN_TIMEOUT"));
      break;
      
    case EV_BEACON_FOUND:
      Serial.println(F("EV_BEACON_FOUND"));
      break;
      
    case EV_BEACON_MISSED:
      Serial.println(F("EV_BEACON_MISSED"));
      break;
      
    case EV_BEACON_TRACKED:
      Serial.println(F("EV_BEACON_TRACKED"));
      break;
      
    case EV_JOINING:
      Serial.println(F("EV_JOINING"));
      break;
      
    case EV_JOINED:
      Serial.println(F("EV_JOINED"));
      //displayNetworkInformation();
      LMIC_setLinkCheckMode(0);
      break;
      
    case EV_JOIN_FAILED:
      Serial.println(F("EV_JOIN_FAILED"));
      break;
      
    case EV_REJOIN_FAILED:
      Serial.println(F("EV_REJOIN_FAILED"));
      break;
      
    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE"));
      break;
      
    case EV_LOST_TSYNC:
      Serial.println(F("EV_LOST_TSYNC"));
      break;
      
    case EV_RESET:
      Serial.println(F("EV_RESET"));
      break;
      
    case EV_RXCOMPLETE:
      Serial.println(F("EV_RXCOMPLETE"));
      break;
      
    case EV_LINK_DEAD:
      Serial.println(F("EV_LINK_DEAD"));
      break;
      
    case EV_LINK_ALIVE:
      Serial.println(F("EV_LINK_ALIVE"));
      break;
      
    case EV_TXSTART:
      Serial.println(F("EV_TXSTART"));
      break;
      
    default:
      Serial.print(F("Unknown event: "));
      Serial.println((unsigned) ev);
      break;
  }
  
}