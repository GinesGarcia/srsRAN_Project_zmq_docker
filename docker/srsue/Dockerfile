FROM ubuntu:22.04

RUN apt-get -y update && apt-get -y install git
RUN apt-get -y install build-essential cmake libfftw3-dev libmbedtls-dev libboost-program-options-dev libconfig++-dev libsctp-dev
RUN apt-get -y install libzmq3-dev iproute2 iputils-ping

# RUN ip netns add ue1

RUN git clone https://github.com/srsRAN/srsRAN_4G.git && \
    cd srsRAN_4G && mkdir build && cd build && cmake .. && \
    make -j `nproc` && make install && cd && \
	srsran_install_configs.sh user && ldconfig

COPY ue.conf /root/.config/srsran/