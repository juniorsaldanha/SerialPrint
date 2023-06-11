#include <Arduino.h>

#include <SerialPrint.hpp>


void setup()
{
    SerialOutput.begin(&Serial, BAUD_RATE, OUTPUT_LEVEL);
    SerialOutput.print(OUTPUT_LEVEL_WARN, "\nESP Booted!\n");

    xTaskCreate(
        [](void *pvParameters)
        {
            SerialOutput.println(OUTPUT_LEVEL_WARN, "SerialOutput.printTask! STARTED");
            while (true)
            {
                vTaskDelay(pdMS_TO_TICKS(1000));
                SerialOutput.print(OUTPUT_LEVEL_DEBUG, "SerialOutput.printTask | Uptime: %d", millis());
                SerialOutput.println(OUTPUT_LEVEL_INFO, "SerialOutput.printTask | Just some info print");
            }
        },
        "SerialOutput.printTask",
        configMINIMAL_STACK_SIZE+1024,
        NULL,
        1,
        NULL);
}

void loop()
{
    SerialOutput.println(OutputLevel::OUTPUT_LEVEL_WARN, "(loop) | to be deleted");
    vTaskDelete(NULL);
}