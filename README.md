# ArduinoEncoder

This library provides to get information about step of motors with encoder. You can use this library for magnetic encoder and also for rotary motor encoder. It supports also Pololu Encoder.

# Usage

Firstly, you need to add [digitalWriteFast](https://github.com/aybarburak/Arduino-Libs/tree/master/digitalWriteFast) Library.
Then, you need to move this Encoder.ino file to your main project location.
After that you need to initalize encoder pins in setup() function:
```
setup_encoder(encoder_pin_M1_a,encoder_pin_M1_b,encoder_pin_M2_a,encoder_pin_M2_b);
```

If you want to get information about encoders, you need to add following line to your code, it returns integer value :
```
get_ticks_M1(); // First Encoder
get_ticks_M2(); // Second Encoder
```

And also if you want to clear this information you should add following link:
```
clear_ticks();
```

# Licence

MIT
