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

#ifndef COMMON_STRUCTURES
#define COMMON_STRUCTURES

union package {
    float float_rep;
    char byte_rep[sizeof(float)];
};

struct SensorReading{
    int id;
    float value;
};

using byte = unsigned char;

namespace headers
{
    // Headers:
    constexpr byte b1 = 0xff; // 255
    constexpr byte b2 = 0x55; // 85
}

namespace ports
{
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
}

namespace actions
{
    // Actions:
    constexpr int get = 0x01; // 1
    constexpr int run = 0x02; // 2
    constexpr int reset = 0x04; // 4
    constexpr int start = 0x05; // 5
}
 
namespace devices
{
    // Devices:
    constexpr int ultrasonic_sensor = 0x01; // 1
    constexpr int motor = 0x0a; // 10
}

#endif /* COMMON_STRUCTURES */