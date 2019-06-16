all: mysqlclient
mysqlclient: mysql_client.c
	gcc -g $< -o $@ -lmysqlclient

.PHONY: clean
clean: 
	rm -rf mysqlclient mysql_client