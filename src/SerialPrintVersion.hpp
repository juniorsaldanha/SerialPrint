/**
 * @file SerialPrintVersion.hpp
 * @author Junior Saldanha (umsaldanha@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SERIAL_PRINT_VERSION_HPP
#define SERIAL_PRINT_VERSION_HPP

#define STRINGIFY0(s) #s
#define STRINGIFY(s) STRINGIFY0(s)

#define FIRMWARE_PROJECT_NAME "SerialPrint - Arduino Library"

/**
 * @brief Firmware version in format MAJOR.MINOR.PATCH
 * 
 */
#define FIRMWARE_VERSION_MAJOR 0
#define FIRMWARE_VERSION_MINOR 0
#define FIRMWARE_VERSION_PATCH 1
#define FIRMWARE_VERSION                                 \
    STRINGIFY(FIRMWARE_VERSION_MAJOR)                    \
    "." STRINGIFY(FIRMWARE_VERSION_MINOR)                \
    "." STRINGIFY(FIRMWARE_VERSION_PATCH)

#endif  // SERIAL_PRINT_VERSION_HPP
