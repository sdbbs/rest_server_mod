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

RUN \
    sed -i 's/# \(.*multiverse$\)/\1/g' /etc/apt/sources.list && \
    apt-get update && \
    apt-get install -y \
    	build-essential \
        git \
	cmake \
	libpq-dev \
	wget \
	autoconf \
	python3 \
	curl \
	libcurl4-openssl-dev

# Authorize SSH Host
RUN mkdir -p /root/.ssh && \
    chmod 0700 /root/.ssh && \
    ssh-keyscan github.com > /root/.ssh/known_hosts

# Add the keys and set permissions
RUN echo "$ssh_prv_key" > /root/.ssh/id_rsa && \
    echo "$ssh_pub_key" > /root/.ssh/id_rsa.pub && \
    chmod 600 /root/.ssh/id_rsa && \
    chmod 600 /root/.ssh/id_rsa.pub



ADD ./project/ /project

RUN cd /root && wget https://github.com/jtv/libpqxx/archive/7.1.0.tar.gz && \
	tar -xvzf 7.1.0.tar.gz && \
	cd libpqxx-7.1.0 && \
	./configure --disable-documentation && make && make install && cp /root/libpqxx-7.1.0/include/pqxx  /usr/include -rf

RUN cd /root &&  git clone https://github.com/open-source-parsers/jsoncpp.git && \
	mkdir jsoncpp-build && \
	cd jsoncpp-build && \
	cmake ../jsoncpp  && \
	make && \
	make install 



RUN cd /project && if [-d Debug ];then rm Debug -rf;fi && mkdir Debug && cd Debug && cmake -DCMAKE_BUILD_TYPE=Debug .. && make check_git && make restbed_install && make && cp server /usr/bin

RUN cp /project/appserver.conf /etc
ENV LD_LIBRARY_PATH="/usr/local/lib"





# Define default command.
CMD ["/usr/bin/server"]
RUN cp /project/3rd_part/restbed/Debug/library/* /usr/local/lib
#ENV LD_LIBRARY_PATH="/project/3rd_part/restbed/Debug/library/"

#CMD["/bin/bash"]
