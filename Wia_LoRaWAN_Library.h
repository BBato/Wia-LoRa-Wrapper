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


#ifndef WIA_LORA_LIBRARY_H
#define WIA_LORA_LIBRARY_H

#include <Arduino.h>



class Wia_LoRaWAN {

  public:
    Wia_LoRaWAN();
    void onLoop();
    void checkWakeupReason();
    bool sendMessage( String packet );

  private:        

};

#endif