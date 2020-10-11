DAEMON_NAME =alertd
FLAGS=`pkg-config --cflags --libs libnotify glib-2.0`
# If you change the above name, do so in alertd.service too

${DAEMON_NAME}: main.c
	gcc main.c ${FLAGS} -o ${DAEMON_NAME}

run: ${DAEMON_NAME}
	./${DAEMON_NAME}

enable_startup:
	gcc main.c ${FLAGS} -DWILL_RUN_BY_SYSTEMD -o ${DAEMON_NAME}
	sudo cp ${DAEMON_NAME} /usr/local/bin
	sudo chmod +x /usr/local/bin/${DAEMON_NAME}
	sudo cp alertd.service /etc/systemd/system/
	 #give read write to owner, read to group, else no permission
	sudo chmod 644 /etc/systemd/system/alertd.service
	sudo systemctl daemon-reload
	sudo systemctl enable alertd
	sudo systemctl start alertd

clear_systemd_entry:
	sudo systemctl stop alertd
	sudo systemctl disable alertd
	sudo rm /usr/local/bin/alertd
	sudo rm /etc/systemd/system/alertd.service
	sudo systemctl daemon-reload

clean:
	rm ${DAEMON_NAME}

#To stop -> sudo systemctl stop /etc/systemd/system/alertd.service
#To disable -> sudo systemctl disable /etc/systemd/system/alertd.service
