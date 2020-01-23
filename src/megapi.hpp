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

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "datatypes.hpp"


// Placeholder Class
class SimpleSerial
{
public:
    SimpleSerial();
    SimpleSerial(std::string port, unsigned int baud_rate);
    void writeString(std::string s);
    std::string readLine();

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
    package read_value;

    MegapiDriver(std::string port, unsigned int baud_rate);
    std::string char_to_string(byte *bytes);
    float string_bytes_to_float(std::string *bytes);
    void run_dc_motor(byte port, byte speed);
    void get_sensor(byte port, byte device);
    void read_serial_sensor(byte port, byte device, SensorReading *reading);
};