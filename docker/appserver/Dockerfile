#
# example Dockerfile for https://docs.docker.com/engine/examples/postgresql_service/
#

FROM ubuntu:20.04
ENV TZ=Asia/Jerusalem
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Define working directory.
WORKDIR /root




# Set environment variables.
ENV HOME /root

# https://stackoverflow.com/questions/18136389/using-ssh-keys-inside-docker-container
ARG ssh_prv_key
ARG ssh_pub_key

RUN apt-get update && \
    apt-get install -y \
        git \
        openssh-server \
        libmysqlclient-dev

# Authorize SSH Host
RUN mkdir -p /root/.ssh && \
    chmod 0700 /root/.ssh && \
    ssh-keyscan github.com > /root/.ssh/known_hosts

# Add the keys and set permissions
RUN echo "$ssh_prv_key" > /root/.ssh/id_rsa && \
    echo "$ssh_pub_key" > /root/.ssh/id_rsa.pub && \
    chmod 600 /root/.ssh/id_rsa && \
    chmod 600 /root/.ssh/id_rsa.pub

# Install.
RUN \
  sed -i 's/# \(.*multiverse$\)/\1/g' /etc/apt/sources.list && \
  apt-get update && \
  apt-get -y upgrade && \
  apt-get install -y build-essential && \
  apt-get install -y git curl wget unzip vim  autoconf libtool libssl-dev  libcurl4-openssl-dev libpq-dev libwebsockets-dev python3-pip python3-numpy


RUN apt-get update -y
RUN apt-get install cmake -y
RUN apt-get install python3 -y



RUN wget https://github.com/jtv/libpqxx/archive/7.1.0.tar.gz && \
	tar -xvzf 7.1.0.tar.gz && \
	cd libpqxx-7.1.0 && \
	./configure --disable-documentation && make && make install && cp /root/libpqxx-7.1.0/include/pqxx  /usr/include -rf

RUN git clone https://github.com/open-source-parsers/jsoncpp.git && \
	mkdir jsoncpp-build && \
	cd jsoncpp-build && \
	cmake ../jsoncpp  && \
	make && \
	make install 

#RUN git clone --recursive https://github.com/corvusoft/restbed.git
#RUN cd restbed
#RUN mkdir build
#RUN cd build
#RUN touch  /root/restbed/cmake/restbedConfig.cmake.in
#RUN cmake  "-DBUILD_SSL=NO  -DBUILD_SHARED=YES"   /root/restbed/ 
#RUN make -j 8 
#RUN make install
#RUN cd ~/restbed && cp distribution/include/* /usr/local/include/ -rf && cp distribution/library/librestbed*  /usr/local/lib -rf
#RUN export LD_LIBRARY_PATH="/usr/local/lib"



RUN rm /root/ecommerce -rf
RUN git clone git@github.com:yairgd/ecommerce.git
RUN cd /root/ecommerce/algotrade/algosys/ && wget http://fxcodebase.com/bin/forexconnect/1.6.3/ForexConnectAPI-1.6.3-Linux-x86_64.tar.gz &&  tar -xvzf ForexConnectAPI-1.6.3-Linux-x86_64.tar.gz
 

# Define working directory.
#RUN cd /root/ecommerce/algotrade/algosys && ./autogen.sh 
#WORKDIR /root/ecommerce/algotrade/algosys/build
#RUN make clean
#RUN make clean && make  _version.cpp
#RUN make   dataServer  -j 8  && cp  /root/ecommerce/algotrade/etc/dataserver.conf  /etc/   && cp  /root/ecommerce/algotrade/algosys/build/dataServer/dataServer  /usr/local/bin

#RUN cmake --version
#RUN cd /root/ecommerce/algotrade/algosys
#RUN mkdir Debug && cd Debug 


RUN pip3 install -r /root/ecommerce/algotrade/algosys/algo/python/requirements.txt
RUN cd /root/ecommerce/algotrade/algosys && echo `pwd` && mkdir Debug && cd Debug && echo `pwd` && cmake -DCMAKE_BUILD_TYPE=Debug .. &&  make  && cp /root/ecommerce/algotrade/algosys/Debug/dataServer/dataServer /usr/local/bin



ENV LD_LIBRARY_PATH="/usr/local/lib/"
RUN cp /root/ecommerce/algotrade/algosys/Debug/dataServer/dataServer /usr/local/bin

CMD ["/usr/local/bin/dataServer"]




# Define default command.
#CMD ["bash"]
