all:
	gcc -Wall -O3 -c -Bshareable xcmd.c -o xcmd.so -lev
	gcc -I/usr/include/json-c -Wall -O3 example.c xcmd.c -o example -ljson-c -lev

test:
	-echo '{ "hi" : "1" }' | ./example
	-echo '{ "argv" : "1" , "envp" : "1" }' | ./example
	-echo '{ "argv" : [ "/bin/ls", "-al" ], "envp" : [ "HI=1", "HELLO=2" ], "type" : "sync" }' | ./example
	-echo '{ "argv" : [ "/bin/ls", "-al" ], "envp" : [ "HI=1", "HELLO=2" ], "type" : "sync", "run" : "none" }' | ./example
	-echo '{ "argv" : [ "/bin/ls", "-al" ], "envp" : [ "HI=1", "HELLO=2" ], "type" : "sync", "run" : "cb" }' | ./example
	-echo '{ "argv" : [ "/bin/ls", "-al" ], "envp" : [ "HI=1", "HELLO=2" ], "type" : "sync", "run" : "exec" }' | ./example
	-echo '{ "argv" : [ "/bin/ls", "-al" ], "envp" : [ "HI=1", "HELLO=2" ], "type" : "sync", "run" : "fork" }' | ./example
	-echo '{ "argv" : [ "/bin/ls", "-al" ], "envp" : [ "HI=1", "HELLO=2" ], "type" : "sync", "run" : "pthread" }' | ./example

jsonc:
	curl https://s3.amazonaws.com/json-c_releases/releases/json-c-0.11.tar.gz | tar zxvf - && cd json-c-0.11 && ./configure --prefix=/usr && make && make install

libev:
	cvs -z3 -d :pserver:anonymous@cvs.schmorp.de/schmorpforge co libev && cd libev && bash autogen.sh && ./confgure --prefix=/usr && make && make install


clean:
	rm -f example
