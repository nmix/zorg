 #!/bin/sh
 mkdir -p tmp/zmq
 cd tmp/zmq
 curl -L -o zmq.tar.gz https://github.com/zeromq/zeromq4-1/releases/download/v4.1.5/zeromq-4.1.5.tar.gz
 tar -xf zmq.tar.gz
 cd zeromq-*
 ./configure
 make
 make install
