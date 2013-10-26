all:
	gcc -Wall -O3 -c -Bshareable xcmd.c -o xcmd.so
	gcc -I/usr/include/json-c -Wall -O3 example.c xcmd.c -o example -ljson-c

test:
	-echo '{ "hi" : "1" }' | ./example
	-echo '{ "argv" : "1" , "envp" : "1" }' | ./example
	-echo '{ "argv" : [ "ls", "-al" ], "envp" : [ "HI=1", "HELLO=2" ], "type" : "sync" }' | ./example

jsonc:
	curl https://s3.amazonaws.com/json-c_releases/releases/json-c-0.11.tar.gz | tar zxvf && cd json-c-0.11 && ./configure --prefix=/usr && make && make install

clean:
	rm -f example
