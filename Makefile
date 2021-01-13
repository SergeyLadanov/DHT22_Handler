CC = gcc
bin_dir = Bin
app_name = hdht
core_src = Core/Src
core_inc = Core/Inc
dht_src = Drivers/DHT/Src
dht_inc = Drivers/DHT/Inc

$(shell mkdir -p $(bin_dir))

app:	$(bin_dir)/main.o $(bin_dir)/dht.o $(bin_dir)/dht_if.o
	$(CC) $(bin_dir)/main.o $(bin_dir)/dht.o $(bin_dir)/dht_if.o -lwiringPi -lpthread -o $(bin_dir)/$(app_name)

$(bin_dir)/main.o:	$(core_src)/main.c
	$(CC) -Wall -E $(core_src)/main.c -lwiringPi -lpthread -o $(bin_dir)/main.i
	$(CC) -Wall -S $(bin_dir)/main.i -lwiringPi -lpthread -o $(bin_dir)/main.s
	$(CC) -Wall -c $(core_src)/main.c -lwiringPi -lpthread -o $(bin_dir)/main.o

$(bin_dir)/dht.o:      $(dht_src)/dht.c $(dht_inc)/dht.h
	$(CC) -Wall -E $(dht_src)/dht.c -I $(dht_inc) -lwiringPi -lpthread -o $(bin_dir)/dht.i
	$(CC) -Wall -S $(bin_dir)/dht.i -lwiringPi -lpthread -o $(bin_dir)/dht.s
	$(CC) -Wall -c $(dht_src)/dht.c -I $(dht_inc) -lwiringPi -lpthread -o $(bin_dir)/dht.o

$(bin_dir)/dht_if.o:      $(core_src)/dht_if.c $(core_inc)/dht_if.h
	$(CC) -Wall -E $(core_src)/dht_if.c -I $(core_inc) -I $(dht_inc)  -lwiringPi -lpthread -o $(bin_dir)/dht_if.i
	$(CC) -Wall -S $(bin_dir)/dht_if.i -lwiringPi -lpthread -o $(bin_dir)/dht_if.s
	$(CC) -Wall -c $(core_src)/dht_if.c -I $(core_inc) -I $(dht_inc) -lwiringPi -lpthread -o $(bin_dir)/dht_if.o

clean:
	rm -r $(bin_dir)

