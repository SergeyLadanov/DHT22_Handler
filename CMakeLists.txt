cmake_minimum_required(VERSION 3.5)

# Название проекта
project(hdht)

# Переменная с именем исполняемого файла
set(EXECUTABLE ${PROJECT_NAME})

# Язык C и C++
enable_language(CXX C)

# Добавление файлов исъодного кода
add_executable(
    ${EXECUTABLE} 
    Core/Src/main.cpp
    Core/Src/MQTT_Client.cpp
    Core/Src/datacontrol.c
    Core/Src/dht_if.c
    Drivers/DHT/Src/dht.c
    Drivers/DSP_Filters/Src/dsp_filters.c
    Drivers/MQTT_Packet/MQTTConnectClient.c
    Drivers/MQTT_Packet/MQTTConnectServer.c
    Drivers/MQTT_Packet/MQTTDeserializePublish.c
    Drivers/MQTT_Packet/MQTTFormat.c
    Drivers/MQTT_Packet/MQTTPacket.c
    Drivers/MQTT_Packet/MQTTSerializePublish.c
    Drivers/MQTT_Packet/MQTTSubscribeClient.c
    Drivers/MQTT_Packet/MQTTSubscribeServer.c
    Drivers/MQTT_Packet/MQTTUnsubscribeClient.c
    Drivers/MQTT_Packet/MQTTUnsubscribeServer.c
    Drivers/TCP_Client/Src/TCP_Client.cpp
)

# Директории с *.h файлами
include_directories(
    Core/Inc
    Drivers/DHT/Inc
    Drivers/DSP_Filters/Inc
    Drivers/TCP_Client/Inc
    Drivers/MQTT_Packet
)

# Флаги компиляции
add_compile_options(
    $<$<COMPILE_LANGUAGE:CXX>:
    -fno-rtti 
    -fno-exceptions 
    $<$<CONFIG:Debug>:
    -Og>
    $<$<CONFIG:Release>:
    -Os>> 
    $<$<COMPILE_LANGUAGE:C>:
    $<$<CONFIG:Debug>:
    -Og>
    $<$<CONFIG:Release>:
    -Os>>
)

# Для Linux
if (UNIX)
    target_link_libraries(
        ${EXECUTABLE}  
        PUBLIC
        pthread
        wiringPi
    )
endif ()

                    