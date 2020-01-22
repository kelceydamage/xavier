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
#include <iomanip>
#include <thread>         // std::this_thread::sleep_for
#include <chrono> 
#include <cstdint>
#include <cstdio>

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

// Headers:
constexpr int header1 = 0xff; // 255
constexpr int header2 = 0x55; // 85

// Ports:
constexpr int motor1a = 0x01; // 1
constexpr int motor2a = 0x02; // 2
constexpr int motor3a = 0x03; // 3
constexpr int motor4a = 0x04; // 4
constexpr int sensor1 = 0x05; // 5
constexpr int sensor2 = 0x06; // 6
constexpr int sensor3 = 0x07; // 7
constexpr int sensor4 = 0x08; // 8
constexpr int motor1b = 0x09; // 9
constexpr int motor2b = 0xa0; // 10
constexpr int motor3b = 0xa1; // 11
constexpr int motor4b = 0xa2; // 12

// Actions:
constexpr int get = 0x01; // 1
constexpr int run = 0x02; // 2
constexpr int reset = 0x04; // 4
constexpr int start = 0x05; // 5

// Devices:
constexpr int ultrasonic_sensor = 0x01; // 1
constexpr int motor = 0x0a; // 10


union {
    float float_rep;
    char byte_rep[sizeof(float)];
    int int_rep;
} package;

typedef struct {
    int id;
    float value;
} SensorReading;


// Placeholder Class
class SimpleSerial
{
public:
    SimpleSerial();

    SimpleSerial(std::string port, unsigned int baud_rate): io(), serial(io, port)
    /*
     * Constructor.
     * \param port device name, example "/dev/ttyUSB0" or "COM4"
     * \param baud_rate communication speed, example 9600 or 115200
     * \throws boost::system::system_error if cannot open the
     * serial device
     */
    {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    void writeString(std::string s)
    /*
     * Write a string to the serial device.
     * \param s string to write
     * \throws boost::system::system_error on failure
     */
    {
        boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
    }

    std::string readLine()
    /*
     * Blocks until a line is received from the serial device.
     * Eventual '\n' or '\r\n' characters at the end of the string are removed.
     * \return a string containing the received line
     * \throws boost::system::system_err
     */
    {
        /*
         * Reading data char by char, code is optimized for simplicity, not speed
         * Will update to a streaming method that does not block. Streams can be
         * organized by sensor id, so continual background readings can be placed
         * buffers.
         */
        char c;
        std::string result;
        int count = 0;
        for(;;)
        {
            boost::asio::read(serial,boost::asio::buffer(&c,1));
            switch(c)
            {
                case '\r':
                    break;
                case '\n':
                    return result;
                default:
                    std::cout << count << " " << (int)c << std::endl;
                    result+=c;
            }
            count++;
        }
    }

private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
};


class MegapiDriver
{
public:
    SimpleSerial *serial;
    std::string port;
    unsigned int baud_rate;

    MegapiDriver(std::string port, unsigned int baud_rate)
    {
        this->port = port;
        this->baud_rate = baud_rate;
        this->serial = new SimpleSerial(this->port, this->baud_rate);
    }

    std::string char_to_string(unsigned char *bytes)
    /* 
     * Convert char array of bytes to string for serial communication.
     * Serialize.
     */
    {
        std::string string;
        for (int i = 0; i < 9; i++)
        {
            char n = bytes[i];
            std::cout << (int)n << " ";
            string.push_back(n);
        }
        std::cout << std::endl;
        return string;
    }

    float string_bytes_to_float(std::string *bytes) 
    /* 
     * Convert string of bytes from serial communication into a float.
     * Deserialize.
     */
    {
        for (int i = 0; i < (*bytes).size(); i++)
        {
            package.byte_rep[i] = bytes->at(i); // large 0x43 0xc8 0x00 0x00 (reverse order)
        }
        return package.float_rep;
    }

    void run_dc_motor(unsigned char port, unsigned char speed) 
    /**************************************************
        ff 55 len idx action device port slot data a
        0  1  2   3   4      5      6    7    8
    ***************************************************/
    /* 
     * package: {ff, 55, len, idx, action, device, port, slot, data, a}
     */
    {
        std::string package;
        unsigned char bytes[] = {
            header1, header2, 0x06, 0x00, run, motor, port, speed, 0x00
        };
        this->serial->writeString(this->char_to_string(bytes));
    }

    void get_sensor(int port, int device)
    /**************************************************
        ff 55 len idx action device port
        0  1  2   3   4      5      6   
    ***************************************************/
    /* 
     * package: {ff, 55, len, idx, action, device, port}
     */
    {
        std::string package;
        char idx = ((port<<4) + device) & 0xff;
        unsigned char bytes[] = {
            header1, header2, 0x04, idx, 0x01, 0x01, port
        };
        this->serial->writeString(this->char_to_string(bytes));
    }

    void read_serial_sensor(int port, int device, SensorReading *reading)
    /**************************************************
        ff 55 idx ? byte1 byte2 byte3 byte4
        0  1  2   3 4     5     6     7 
    ***************************************************/
    /* 
     * package: {h1, h2, 0, 2, b0, b1, b2, b3}
     * last 4 bytes are a 4 byte float.
     * returns sensor_reading<struct> of {idx, value}.
     */
    {
        this->get_sensor(port, device);
        std::string result = this->serial->readLine();
        float value = this->string_bytes_to_float(&(result.substr(5, 4)));
        reading->id = result[2];
        reading->value = value;
    }
};


int main(int argc, char* argv[])
{
    try {
        SensorReading ultrasonic_sensor_reading;

        MegapiDriver driver("/dev/ttyTHS0", 115200);
        std::cout << std::endl;

        std::cout << "Starting ...\n";
        std::cout << "Connecting to: " << "/dev/ttyTHS0\n";

        std::cout << "Writing ... \n";
        driver.run_dc_motor(motor1a, 255);

        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Writing ... \n";
        driver.run_dc_motor(motor1a, 0);

        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Reading ...\n";
        driver.read_serial_sensor(sensor4, ultrasonic_sensor, &ultrasonic_sensor_reading);

        std::cout << "Sensor: " << ultrasonic_sensor_reading.id << ", Value: ";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << (float)ultrasonic_sensor_reading.value << std::endl;

        std::cout << "Done" << std::endl;

    } catch(boost::system::system_error& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
}
