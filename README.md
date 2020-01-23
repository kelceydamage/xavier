# xavier
Megapi driver, uart, i2c, general robotics controls.

# Example
```cpp
int main(int argc, char* argv[])
{
    try {
        MegapiDriver driver(config::terminal, config::baud_rate);
        Movement movement(&driver);
        Sensor ultrasonic_sensor(
            &driver, devices::ultrasonic_sensor, ports::sensor4
        );

        movement.forward(255, 1);

        movement.stop(1);

        movement.reverse(255, 1);

        movement.stop(1);

        int n = 10;
        while (n > 0)
        {
            Debug::print_sensor_reading(ultrasonic_sensor.read());
            n--;
        }

        std::exit(0);

    } catch(boost::system::system_error& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
}
```

# Configuration
```cpp
namespace config
{
    int baud_rate = 115200;
    std::string terminal = "/dev/ttyTHS0";
}
```
