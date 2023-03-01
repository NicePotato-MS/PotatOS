SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"
 
export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./default-host.sh)}

# lazy method of ridding (most of) the garbage before sysroot by using a folder in root called "proot"
# make sure to run as su!
# change this to the path to your gcc or whatever
mkdir -p /proot/
export SUINC=/home/micah/opt/cross/bin
# if you know how to modify that junk before sysroot, please lmk!

export AR=${SUINC}/${HOST}-ar
export AS=${SUINC}/${HOST}-as
export CC=${SUINC}/${HOST}-gcc
 
export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include
 
export CFLAGS='-O2 -g'
export CPPFLAGS=''
 
# Configure the cross-compiler to use the desired system root.
export SYSROOT="/proot/sysroot"
export CC="$CC --sysroot=$SYSROOT"
 
# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
  export CC="$CC -isystem=$INCLUDEDIR"
fi