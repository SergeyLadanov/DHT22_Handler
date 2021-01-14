# Вспомогательные переменные
# Имя компилятора
CC = gcc
# Директория с выходными файлами
bin_dir = Bin
# Имя исполняемого файла
app_name = hdht
# Флаги компиляции
flags = -lwiringPi -lpthread

# Пути с исходниками
DIR1 = Core
DIR2 = Drivers/DHT
DIR3 = Drivers/Median_Filter

# Формирование массива каталогов проекта
path_list = $(DIR1) $(DIR2)
header_list = $(foreach d, $(path_list), -I $d/Inc)

# Список объектных файлов
objects = $(bin_dir)/main.o $(bin_dir)/dht.o $(bin_dir)/dht_if.o

$(shell mkdir -p $(bin_dir))

# Правило для сборки исполняемого файла
app:	$(objects)
	$(CC) $(objects) $(flags) -o $(bin_dir)/$(app_name)

# Правило сборки для каталога DIR1
$(bin_dir)/%.o: $(DIR1)/Src/%.c $(wildcard $(DIR1)/Inc/*.h)
	$(CC) -Wall -c $< $(header_list) $(flags) -o $@

# Правило сборки для каталога DIR2
$(bin_dir)/%.o: $(DIR2)/Src/%.c $(wildcard $(DIR2)/Inc/*.h)
	$(CC) -Wall -c $< $(header_list) $(flags) -o $@

# Правило для очистки
clean:
	rm -r $(bin_dir)

