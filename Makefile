CC = gcc
bin_dir = Bin
app_name = hdht
core_src = Core
core_inc = Core
dht_src = Drivers/DHT
dht_inc = Drivers/DHT

$(shell mkdir -p $(bin_dir))

app:	$(bin_dir)/main.o $(bin_dir)/dht.o
	$(CC) $(bin_dir)/main.o $(bin_dir)/dht.o -lwiringPi -lpthread -o $(bin_dir)/$(app_name)

$(bin_dir)/main.o:	$(core_src)/main.c
	$(CC) -Wall -E $(core_src)/main.c -lwiringPi -lpthread -o $(bin_dir)/main.i
	$(CC) -Wall -S $(bin_dir)/main.i -lwiringPi -lpthread -o $(bin_dir)/main.s
	$(CC) -Wall -c $(core_src)/main.c -lwiringPi -lpthread -o $(bin_dir)/main.o

$(bin_dir)/dht.o:      $(dht_src)/dht.c $(dht_inc)/dht.h
	$(CC) -Wall -E $(dht_src)/dht.c -lwiringPi -lpthread -o $(bin_dir)/dht.i
	$(CC) -Wall -S $(bin_dir)/dht.i -lwiringPi -lpthread -o $(bin_dir)/dht.s
	$(CC) -Wall -c $(dht_src)/dht.c -lwiringPi -lpthread -o $(bin_dir)/dht.o

clean:
	rm -r $(bin_dir)
