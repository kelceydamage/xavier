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
#include <thread>         // std::this_thread::sleep_for
#include <chrono> 
#include "platform/datatypes.hpp"
#include "movement.hpp"


Movement::Movement(Driver *driver)
{
    this->driver = driver;
}

void Movement::execute(short int speed, int time)
{
    this->driver->run_dc_motor(ports::motor1a, speed);
    this->driver->run_dc_motor(ports::motor2a, speed);
    std::this_thread::sleep_for(std::chrono::seconds(time));
}

void Movement::forward(short int speed, int time) 
{
    this->execute(speed, time);
}

void Movement::stop(int time) 
{
    this->execute(0, time);
}

void Movement::reverse(short int speed, int time) 
{
    this->execute(-speed, time);
}