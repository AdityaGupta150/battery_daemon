all: alerter.c
	gcc alerter.c `pkg-config --cflags --libs libnotify glib-2.0` -o alerter

run: all
	./alerter

clean:
	rm alerter
