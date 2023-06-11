/**
 * @file SerialPrint.cpp
 * @author Junior Saldanha (umsaldanha@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-06-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <SerialPrint.hpp>

/**
 * @brief Global instance of SerialPrint class.
 *
 * @return SerialPrint
 */
SerialPrint SerialOutput(&Serial);

/**
 * @brief Construct a new Serial Print:: Serial Print object
 *
 */
SerialPrint::SerialPrint()
    : _serialPort(&Serial),
      _baudRate(115200),
      _outputLevel(OUTPUT_LEVEL_DEBUG) {}

/**
 * @brief Construct a new Serial Print:: Serial Print object
 *
 * @param serialPort
 */
SerialPrint::SerialPrint(HardwareSerial* serialPort)
    : _serialPort(serialPort),
      _baudRate(115200),
      _outputLevel(OUTPUT_LEVEL_DEBUG) {}

/**
 * @brief Destroy the Serial Print:: Serial Print object
 *
 */
SerialPrint::~SerialPrint() {
    _serialPort->end();

    if (tsFunction != nullptr) {
        free(tsFunction);
        tsFunction = nullptr;
    }

    _serialPort = nullptr;

    free(_serialPort);

    _baudRate = 0;

    _outputLevel = OUTPUT_LEVEL_DEBUG;

    _datetimeISO = false;

    _timestamp = false;

    free(_outputLevelString);

    free(this);
}

/**
 * @brief Initialize serial port and set baud rate and output level.
 *
 * @param serialPort
 * @param baudRate
 * @param outputLevel
 */
void SerialPrint::begin(HardwareSerial* serialPort, uint32_t baudRate,
                        OutputLevel outputLevel) {
    _serialPort = serialPort;
    _baudRate = baudRate;
    _serialPort->begin(_baudRate);
    setOutputLevel(outputLevel);
}

/**
 * @brief Set the Output Level object
 *
 * @param outputLevel
 */
void SerialPrint::setOutputLevel(OutputLevel outputLevel) {
    _outputLevel = outputLevel;
}

/**
 * @brief Set the Timestamp Function object
 *
 * @param tsFunction
 */
void SerialPrint::setTimestampFunction(void* tsFunction) {
    this->tsFunction = tsFunction;
}

/**
 * @brief Set the Datetime ISO object
 *
 * @param enable
 */
void SerialPrint::setDatetimeISO(bool enable) {
    _datetimeISO = enable;
}

/**
 * @brief Set the Timestamp object
 *
 * @param enable
 */
void SerialPrint::setTimestamp(bool enable) {
    _timestamp = enable;
}

/**
 * @brief Get the Date ISO8601 object using the timestamp function.
 * The format is: YYYY-MM-DDThh:mm:ss
 *
 * @return const char*
 */
const char* SerialPrint::getDateISO8601() {
    if (tsFunction == nullptr)
        return "";
    unsigned long timestamp = ((uint32_t(*)(void))tsFunction)();
    int year = floor(timestamp / 31536000 + 1970);
    int month = floor((timestamp % 31536000) / 2628000 + 1);
    int day = floor(((timestamp % 31536000) % 2628000) / 86400 + 1);
    int hour = floor((((timestamp % 31536000) % 2628000) % 86400) / 3600);
    int minute =
        floor(((((timestamp % 31536000) % 2628000) % 86400) % 3600) / 60);
    int second =
        floor(((((timestamp % 31536000) % 2628000) % 86400) % 3600) % 60);

    char* ret = (char*)malloc(30);
    sprintf(ret, "%04d-%02d-%02dT%02d:%02d:%02d", year, month, day, hour,
            minute, second);
    return ret;
}

/**
 * @brief Print to serial port using printf style formatting.
 *
 * @param format
 * @param ...
 */
void SerialPrint::printf(const char* format, ...) {
    char loc_buf[64];
    char* temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if (len < 0) {
        va_end(arg);
        return;
    };
    if (len >=
        (int)sizeof(
            loc_buf)) {  // comparation of same sign type for the compiler
        temp = (char*)malloc(len + 1);
        if (temp == NULL) {
            va_end(arg);
            return;
        }
        len = vsnprintf(temp, len + 1, format, arg);
    }
    va_end(arg);
    len = _serialPort->write((uint8_t*)temp, len);
}

/**
 * @brief Check if output level is greater than or equal to the set output
 * level, then concatenate the output level string and the message and call
 * printf to print to serial port.
 *
 * @param outputLevel
 * @param format
 * @param ...
 */
void SerialPrint::print(OutputLevel outputLevel, const char* format, ...) {
    if (outputLevel >= _outputLevel) {
        char loc_buf[64];
        char* temp = loc_buf;
        va_list arg;
        va_list copy;
        va_start(arg, format);
        va_copy(copy, arg);
        int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
        va_end(copy);
        if (len < 0) {
            va_end(arg);
            return;
        };
        if (len >=
            (int)sizeof(
                loc_buf)) {  // comparation of same sign type for the compiler
            temp = (char*)malloc(len + 1);
            if (temp == NULL) {
                va_end(arg);
                return;
            }
            len = vsnprintf(temp, len + 1, format, arg);
        }
        va_end(arg);
        if (tsFunction == nullptr)
            _serialPort->printf("[%s] | %s", _outputLevelString[outputLevel],
                                temp);
        else if (_timestamp && _datetimeISO)
            _serialPort->printf(
                "[%s] | %lu | %s | %s", _outputLevelString[outputLevel],
                ((uint32_t(*)(void))tsFunction)(), getDateISO8601(), temp);
        else if (_timestamp && !_datetimeISO)
            _serialPort->printf("[%s] | %lu | %s",
                                _outputLevelString[outputLevel],
                                ((uint32_t(*)(void))tsFunction)(), temp);
        else if (!_timestamp && _datetimeISO)
            _serialPort->printf("[%s] | %s | %s",
                                _outputLevelString[outputLevel],
                                getDateISO8601(), temp);
    }
}

/**
 * @brief Print to serial port using printf style formatting and add a new line
 * at the end.
 *
 * @param outputLevel
 * @param message
 */
void SerialPrint::println(OutputLevel outputLevel, String message) {
    print(outputLevel, "%s\n", message.c_str());
}

/**
 * @brief Print to serial port using printf style formatting and add a new line
 * at the end.
 *
 * @param outputLevel
 * @param message
 */
void SerialPrint::println(OutputLevel outputLevel, const char* message) {
    print(outputLevel, "%s\n", message);
}
