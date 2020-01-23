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
#include "drivers/uart_megapi.hpp"
#include "config.hpp"
#include "movement.hpp"
#include "sensors.hpp"


int main(int argc, char* argv[])
{
    try {
        MegapiDriver driver(config::terminal, config::baud_rate);
        Movement movement(&driver);
        Sensor ultrasonic_sensor(
            &driver, devices::ultrasonic_sensor, ports::sensor4
        );
        Sensor compass(
            &driver, devices::compass, ports::sensor3
        );

        std::cout << std::endl;

        std::cout << "Starting ...\n";
        std::cout << "Connecting to: " << config::terminal << " @ ";
        std::cout << "0.0087 ms" << std::endl;

        std::cout << "Forward ... \n";
        movement.forward(255, 1);

        std::cout << "Stop ... \n";
        movement.stop(1);

        std::cout << "Reverse ... \n";
        movement.reverse(255, 1);

        std::cout << "Stop ... \n";
        movement.stop(1);

        int n = 10;
        while (n > 0)
        {
            Debug::print_sensor_reading(ultrasonic_sensor.read());
            Debug::print_sensor_reading(compass.read());
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