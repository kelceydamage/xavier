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
#include <iomanip>
#include "platform/datatypes.hpp"
#include "sensors.hpp"


Sensor::Sensor(Driver *driver, int sensor, byte port)
{
    this->driver = driver;
    this->id = sensor;
    this->port = port;
}

SensorReading* Sensor::read()
{
    this->driver->read_serial_sensor(
        this->port,
        this->id,
        &(this->reading)
    );
    return &(this->reading);
}