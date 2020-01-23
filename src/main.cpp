/* ------------------------------------------------------------------------ 79->
 * Author: Kelcey Damage
 * c++: 17
 * Doc
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <boost/asio.hpp>
#include <iostream>
#include "debug.hpp"
#include "megapi.hpp"
#include "config.hpp"
#include "movement.hpp"
#include "sensors.hpp"

//
//def motorRun(self,port,speed):
//  self.__writePackage(bytearray(
//    [0xff,0x55,0x6,0x0,0x2,0xa,port]+self.short2bytes(speed))
//  )

/*
def ultrasonicSensorRead(self,port,callback):
    self.__writeRequestPackage(1,port,callback)


    self.__writePackage(
        bytearray([0xff,0x55,0x6,0x0,0x2,0xa,port]+self.short2bytes(speed))
        )

def __writeRequestPackage(self,deviceId,port,callback):
    extId = ((port<<4)+deviceId)&0xff
    self.__doCallback(extId,callback)
    self.__writePackage(bytearray([0xff,0x55,0x4,extId,0x1,deviceId,port]))

def __writePackage(self,pack):
    self.device.writePackage(pack)

def writePackage(self,package):
    self.ser.write(package)
    sleep(0.01)

def onRead(v):
	print("distance:"+str(v)+" cm");

#define PORT1A  PORT_1
#define PORT1B  PORT_9
#define PORT2A  PORT_2
#define PORT2B  PORT_10
#define PORT3A  PORT_3
#define PORT3B  PORT_11
#define PORT4A  PORT_4
#define PORT4B  PORT_12

#define ULTRASONIC_SENSOR      1
define MOTOR                  10

#define SET_MEGAPI_MODE      0x12
#define GET_MEGAPI_MODE      0x72

Actions:
#define GET 1
#define RUN 2
#define RESET 4
#define START 5
*/

// ff 55 len idx action device port  slot  data a
/* Serial Package
 * 0 // char header1
 * 1 // char header2
 * 2 // char len // values after len in package
 * 3 // char idx
 * 4 // char action
 * 5 // char device
 * 6 // char port
 * 7 // char slot // speed for DC motor. If encoder and PORT = 0, then motor slot.
 * 8 // char a // speed for Encoder motor
 */
/* Sensor
 * 0xff = static // 255 // header 
 * 0x55 = static // 85 // header
 * 0x04 = static // 4
 * 0x00 = extId = ((port<<4)+deviceId)&0xff // 81
 * 0x01 = static
 * 0x0a = deviceId = 1
 * 0x05 = Port = (5, 6, 7, 8)
*/

/*
union
{
  uint8_t byteVal[4];
  float floatVal;
  long longVal;
}val;

void sendFloat(float value)
{ 
  writeSerial(2);
  val.floatVal = value;
  writeSerial(val.byteVal[0]);
  writeSerial(val.byteVal[1]);
  writeSerial(val.byteVal[2]);
  writeSerial(val.byteVal[3]);
}
writeHead();
writeSerial(command_index);
sendFloat(value);

package: {h1, h2, 0, 2, v0, v1, v2, v3}
*/

uint8_t command_index = 0; // uint idx


int main(int argc, char* argv[])
{
    try {
        MegapiDriver driver(config::terminal, config::baud_rate);
        Movement movement(&driver);
        Sensor ultrasonic_sensor(
            &driver, devices::ultrasonic_sensor, ports::sensor4
        );

        std::cout << std::endl;

        std::cout << "Starting ...\n";
        std::cout << "Connecting to: " << config::terminal << " @ ";
        std::cout << "0.0087 ms" << std::endl;

        std::cout << "Writing ... \n";
        movement.forward(3000, 1);

        std::cout << "Writing ... \n";
        movement.stop(1);

        std::cout << "Writing ... \n";
        movement.reverse(3000, 1);

        std::cout << "Writing ... \n";
        movement.stop(1);

        std::cout << "Reading ...\n";
        int n = 10;
        while (n > 0)
        {
            Debug::print_sensor_reading(ultrasonic_sensor.read());
            n--;
        }
        std::cout << "Done" << std::endl;
        std::exit(0);

    } catch(boost::system::system_error& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
}