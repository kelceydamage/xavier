# xavier
Megapi driver, uart, i2c, general robotics controls.

# Example
```cpp
int main(int argc, char* argv[])
{
    try {
        MegapiDriver driver(config::terminal, config::baud_rate);
        SensorReading ultrasonic_sensor_reading;

        driver.run_dc_motor(ports::motor1a, 255);

        driver.run_dc_motor(ports::motor1a, 0);

        int n = 10;
        while (n > 0)
        {
            driver.read_serial_sensor(
                ports::sensor4,
                devices::ultrasonic_sensor,
                &ultrasonic_sensor_reading
            );
            Debug::print_sensor_reading(&ultrasonic_sensor_reading);
            n--;
        }

    } catch(boost::system::system_error& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
}
```
