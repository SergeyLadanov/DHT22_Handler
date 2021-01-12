CC = gcc
bin_dir = Bin
app_name = hdht

$(shell mkdir -p $(bin_dir))

app:	$(bin_dir)/app.o
	$(CC) $(bin_dir)/app.o -lwiringPi -lpthread -o $(bin_dir)/$(app_name)

$(bin_dir)/app.o:	app.c
	$(CC) -Wall -E app.c -lwiringPi -lpthread -o $(bin_dir)/app.i
	$(CC) -Wall -S $(bin_dir)/app.i -lwiringPi -lpthread -o $(bin_dir)/app.s
	$(CC) -Wall -c app.c -lwiringPi -lpthread -o $(bin_dir)/app.o
clean:
	rm -r $(bin_dir)
