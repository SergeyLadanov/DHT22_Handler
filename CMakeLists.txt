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
    Core/Src/datacontrol.c
    Core/Src/dht_if.c
    Drivers/DHT/Src/dht.c
    Drivers/DSP_Filtres/Src/dsp_filtres.c
)

# Директории с *.h файлами
include_directories(
    Core/Inc
    Drivers/DHT/Inc
    Drivers/DSP_Filtres/Inc
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

                    