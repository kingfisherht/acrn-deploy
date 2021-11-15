#!/usr/bin/env bash
USER_PASS="intel123"

check_return_code () {
  ret=$?
  message=$1
  if [[ $ret != 0 ]]; then
     echo "ERROR: $message"
     exit $ret
  fi

}
run_with_sudo_if_required () {
    echo "INFO: called \"$@\""

    if [ "$EUID" -ne 0 ]
        then
            eval "echo \"${USER_PASS}\" | sudo -S $@"
        else
            eval "$@"
    fi
}

function apt_update {
    run_with_sudo_if_required apt-get -y update
    check_return_code "Can't perform apt update"
}

function install_host_dependencies {
  #install acrn tools
  run_with_sudo_if_required apt-get -y install  gcc \
     git \
     make \
     libssl-dev \
     libpciaccess-dev \
     uuid-dev \
     libsystemd-dev \
     libevent-dev \
     libxml2-dev \
     libxml2-utils \
     libusb-1.0-0-dev \
     python3 \
     python3-pip \
     libblkid-dev \
     e2fslibs-dev \
     pkg-config \
     libnuma-dev \
     liblz4-tool \
     flex \
     bison \
     xsltproc \
     clang-format \
     libelf-dev \
     vim \
     net-tools \
     ssh \
     cpuid \

   check_return_code "\`apt-get install base tools\` failed"

   #install python
   export http_proxy="http:://child-prc.intel.com:913"
   export https_proxy="https:://child-prc.intel.com:913"
   pip3 install lxml xmlschema
   pushd ~/acrn_deploy
   tar xvf msr-tools_1.3.orig.tar.gz && cd msr-tools-1.3
   make -j8 
   run_with_sudo_if_required make install -j8
   check_return_code "\`install msr"
   popd
   run_with_sudo_if_required cp iasl /usr/bin/
   run_with_sudo_if_required cp OVMF.fd /usr/share/acrn/bios/

}
function build_acrn_hyperviosr {
    cd ~
    cp ./acrn_deploy/config .gitconfig
    if [[ -d "./codebase" ]]; then
    cd codebase
    else
    mkdir codebase && cd codebase
    fi
    git clone https://github.com/projectacrn/acrn-hypervisor.git
    cd acrn-hypervisor && git checkout -b acrn2.5 remotes/origin/release_2.5
    make BOARD=whl-ipc-i5 SCENARIO=industry -j8
    #make BOARD=whl-ipc-i7 SCENARIO=industry -j8
    #make BOARD=tgl-rvp SCENARIO=industry -j8
    if [[ -d "./build/" ]]; then
    echo "Success build"
    fi
    pushd ./build/hypervisor
    run_with_sudo_if_required mkdir -p /boot/acrn
    run_with_sudo_if_required cp acrn.bin /boot/acrn/
    popd
    run_with_sudo_if_required cp ./build/devicemodel/acrn-dm /usr/bin/

}

function network_transform {
    pushd /home/intel/acrn_deploy
    run_with_sudo_if_required cp ./network/*  /lib/systemd/network/
    check_return_code "\`config the acrn network"
    run_with_sudo_if_required cp ./setip.service /lib/systemd/system/
    check_return_code "\`config service"
    popd
    run_with_sudo_if_required systemctl disable NetworkManager
    check_return_code "\`systemctl disable NetworkManager"
    run_with_sudo_if_required systemctl enable systemd-networkd
    check_return_code "\`enable systemd-networkd"
    run_with_sudo_if_required systemctl enable systemd-resolved
    run_with_sudo_if_required systemctl start systemd-resolved
    run_with_sudo_if_required rm /etc/resolv.conf
    run_with_sudo_if_required ln -s /run/systemd/resolve/resolv.conf /etc/resolv.conf
    check_return_code "\`network_transform"
    run_with_sudo_if_required systemctl enable /lib/systemd/system/setip.service   
}

function config_environment {

    echo "intel123" | sudo -S  su
    pushd  /home/intel/acrn_deploy
    cp sudoers /etc/sudoers
    popd
    run_with_sudo_if_required cp sources.list /etc/apt/
    check_return_code "ok for sources"
    run_with_sudo_if_required cp apt.conf /etc/apt/
    check_return_code "ok for apt.conf"
}

function install_kernel {
    echo "install kernel"
    pushd  /home/intel/acrn_deploy
    run_with_sudo_if_required cp bzImage /boot/
    run_with_sudo_if_required tar xvf 5.4.106-PKT-SOS.tar.gz -C /lib/modules/
    check_return_code "install kernel"
    popd
}

function update_grube {
    pushd  /home/intel/acrn_deploy
    UUID=`lsblk -f | grep "ext4" | cut -d " " -f 11`
    echo $UUID
    #sed 's/24d718aa-8d21-45cd-a300-4e0e0cd565de/$UUID/' sed-40_custom
    run_with_sudo_if_required cp 40_custom /etc/grub.d/
    run_with_sudo_if_required update-grub
    popd
}
#config_environment
config_environment
echo "SUCESS: config the proxy"
apt_update
echo "SUCCESS: apt was updated"
install_host_dependencies
echo "SUCCESS: host tools dependencies installed"
#build acrn hypervisor
build_acrn_hyperviosr
echo "SUCCESS: install kernel"
#change the networkmanager to systemd-network
install_kernel
echo "SUCCESS: install kernel"
update_grube
echo "SUCCESS: update_grube"
network_transform
echo "SUCCESS: network transform"
echo "Before reboot pls change the UUID and the appropriate acrn.bin"