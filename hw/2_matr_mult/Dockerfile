FROM python:3.6

COPY src /src
COPY Makefile /Makefile
COPY build.sh /build.sh
COPY clean.sh /clean.sh

WORKDIR .

RUN ["bash", "build.sh"]

