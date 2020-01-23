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
#include "megapi.hpp"


SimpleSerial::SimpleSerial(std::string port, unsigned int baud_rate): io(), serial(io, port)
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

void SimpleSerial::writeString(std::string s)
/*
    * Write a string to the serial device.
    * \param s string to write
    * \throws boost::system::system_error on failure
    */
{
    boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
}

std::string SimpleSerial::readLine()
/*
    * Blocks until a line is received from the serial device.
    * Eventual '\n' or '\r\n' characters at the end of the string are removed.
    * \return a string containing the received line
    * \throws boost::system::system_err
    */
{
    // Reading data char by char, code is optimized for simplicity, not speed
    byte c;
    std::string result;
    for(;;)
    {
        boost::asio::read(serial,boost::asio::buffer(&c,1));
        if (c == '\n')
        {
            return result;
        } 
        else
        {
            result+=c;
        }
    }
}


MegapiDriver::MegapiDriver(std::string port, unsigned int baud_rate)
{
    this->port = port;
    this->baud_rate = baud_rate;
    this->serial = new SimpleSerial(this->port, this->baud_rate);
}

std::string MegapiDriver::char_to_string(byte *bytes)
/* 
    * Convert char array of bytes to string for serial communication.
    * Serialize.
    */
{
    std::string string;
    for (int i = 0; i < bytes[2] + 3; i++)
    {
        byte n = bytes[i];
        std::cout << (int)n << " ";
        string.push_back(n);
    }
    std::cout << std::endl;
    return string;
}

float MegapiDriver::string_bytes_to_float(std::string *bytes) 
/* 
    * Convert string of bytes from serial communication into a float.
    * Deserialize.
    */
{
    for (int i = 0; i < (*bytes).size(); i++)
    {
        this->read_value.byte_rep[i] = bytes->at(i); // large 0x43 0xc8 0x00 0x00 (reverse order)
    }
    return this->read_value.float_rep;
}

void MegapiDriver::run_dc_motor(byte port, byte speed) 
/**************************************************
    ff 55 len idx action device port slot data a
    0  1  2   3   4      5      6    7    8
***************************************************/
/* 
    * package: {ff, 55, len, idx, action, device, port, slot, data, a}
    */
{
    byte bytes[] = {
        headers::b1,
        headers::b2,
        0x06,
        0x00,
        actions::run,
        devices::motor,
        port,
        speed,
        0x00
    };
    this->serial->writeString(this->char_to_string(bytes));
}

void MegapiDriver::get_sensor(byte port, byte device)
/**************************************************
    ff 55 len idx action device port
    0  1  2   3   4      5      6   
***************************************************/
/* 
    * package: {ff, 55, len, idx, action, device, port}
    */
{
    byte idx = ((port<<4) + device) & 0xff;
    byte bytes[] = {
        headers::b1,
        headers::b2,
        0x04,
        idx,
        actions::get,
        device,
        port
    };
    this->serial->writeString(this->char_to_string(bytes));
}

void MegapiDriver::read_serial_sensor(byte port, byte device, SensorReading *reading)
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
    int byte_position = 4;
    int byte_count = 4;
    this->get_sensor(port, device);
    std::string result = this->serial->readLine();
    std::string float_bytes = result.substr(byte_position, byte_count);
    float value = this->string_bytes_to_float(&float_bytes);
    reading->id = result[2];
    reading->value = value;
}