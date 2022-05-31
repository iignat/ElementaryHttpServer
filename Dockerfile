FROM ubuntu:bionic

MAINTAINER Ignat Ikryanov <iignat@mail.ru>

ENV TZ=Europe/Moscow

COPY ./start.sh /tmp/
ADD ./www /var/www

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && \
    apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    mc \
    && \
    apt-get clean && \
    tr -d '\r' </tmp/start.sh >/var/start.sh && \
    chmod +x /var/start.sh && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /tmp
RUN git -c http.sslVerify=false clone https://github.com/iignat/ElementaryHttpServer.git \
    && \
    cmake -S ./ -B ./ElementaryHttpServer \
    && \
    make \
    && \
    mv ./ElementaryServer /bin/ElementaryServer

ENTRYPOINT ["/var/start.sh"]
