HOWTO build Linux kernel for cheshire

Clone cva6-sdk:

    $ cd ${HOME}
    $ pwd
    /home/user
    $ git clone https://github.com/pulp-platform/cva6-sdk
    $ cd cva6-sdk

Checkout the cheshire branch (55c137141b62 "opensbi: Update"):

    $ git checkout 55c137141b62d4b84722d08e0f5d2b9a5af1e77f

Use recomendations from cva6-sdk README.md:

    $ git submodule update --init --recursive
    $ make images

Check images:

    $ ls -1 buildroot/output/images/
    rootfs.cpio
    rootfs.cpio.gz
    vmlinux

Clone linux:

    $ git clone -b v5.10.7-cva6-sdk https://github.com/frantony/linux
    $ cd linux

Build vmlinux:

    $ export BR_BINARIES_DIR=$(cd .. && pwd)/buildroot/output/images
    $ export ARCH=riscv
    $ export CROSS_COMPILE=$(cd .. && pwd)/buildroot/output/host/bin/riscv64-linux-
    $ make cva6_sdk_defconfig
    $ make
    $ ls -1 vmlinux
    vmlinux
