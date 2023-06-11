/**
 * @file SerialPrint.hpp
 * @author Junior Saldanha (esaldanha@ionic.health)
 * @brief
 * @version 0.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SERIAL_PRINT_HPP
#define SERIAL_PRINT_HPP

#include <Arduino.h>
#include <HardwareSerial.h>
/**
 * @brief Output levels
 *
 */
typedef enum {
    OUTPUT_LEVEL_DEBUG = 0,
    OUTPUT_LEVEL_INFO,
    OUTPUT_LEVEL_WARN,
    OUTPUT_LEVEL_ERROR,
    OUTPUT_LEVEL_FATAL
} OutputLevel;

/**
 * @brief SerialPrint class to print messages to serial port using printf style
 * formatting, and with different output levels.
 *
 */
class SerialPrint {
   private:
    HardwareSerial* _serialPort;
    uint32_t _baudRate;
    OutputLevel _outputLevel;
    void* tsFunction = nullptr;
    bool _datetimeISO = false;
    bool _timestamp = false;

    const char* _outputLevelString[5] = {"DEBUG", "INFO ", "WARN ", "ERROR",
                                         "FATAL"};

    void printf(const char*, ...);

    const char* getDateISO8601();

   public:
    SerialPrint();
    explicit SerialPrint(HardwareSerial* serialPort);

    ~SerialPrint();

    void begin(HardwareSerial*, uint32_t, OutputLevel);

    void setOutputLevel(OutputLevel);
    void setTimestampFunction(void* tsFunction);
    void setDatetimeISO(bool enable);
    void setTimestamp(bool enable);

    void print(OutputLevel, const char*, ...);
    void println(OutputLevel, String);
    void println(OutputLevel, const char*);
};

extern SerialPrint SerialOutput;

#endif  // SERIAL_PRINT_HPP
