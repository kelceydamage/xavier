# xavier
Megapi driver, uart, i2c, general robotics controls.

# Example
```cpp
int main(int argc, char* argv[])
{
    try {
        SensorReading ultrasonic_sensor_reading;

        MegapiDriver driver("/dev/ttyTHS0", 115200);

        driver.run_dc_motor(motor1a, 255);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        driver.run_dc_motor(motor1a, 0);

        driver.read_serial_sensor(
            sensor4, 
            ultrasonic_sensor, 
            &ultrasonic_sensor_reading
        );

        std::cout << "Sensor: " << ultrasonic_sensor_reading.id << ", Value: ";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << (float)ultrasonic_sensor_reading.value << std::endl;

    } catch(boost::system::system_error& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
}
```
