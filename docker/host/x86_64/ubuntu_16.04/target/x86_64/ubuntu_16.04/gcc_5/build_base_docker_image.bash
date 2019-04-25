#! /bin/bash

HOSTARCH=x86_64
HOSTOSKIND=ubuntu
HOSTOSVERSION=16.04
HOSTOS=${HOSTOSKIND}_${HOSTOSVERSION}
TARGETARCH=x86_64
TARGETOSKIND=ubuntu
TARGETOSVERSION=16.04
TARGETOS=${TARGETOSKIND}_${TARGETOSVERSION}
COMPILERKIND=gcc
COMPILERVERSION=5
COMPILER=${COMPILERKIND}_${COMPILERVERSION}

SCRIPTPATH=$(cd "$(dirname "$0")"; pwd -P)

SWPATH=$(realpath ${SCRIPTPATH}/../../../../../../../../../..)

# Copy the .dockerignore file from //sw/gpgpu/libcudacxx to //sw/gpgpu.
cp ${SWPATH}/gpgpu/libcudacxx/docker/.dockerignore ${SWPATH}/gpgpu

docker -D build \
  -t libcudacxx_base:host_${HOSTARCH}_${HOSTOS}__target_${TARGETARCH}_${TARGETOS}__${COMPILER} \
  -f ${SWPATH}/gpgpu/libcudacxx/docker/host/${HOSTARCH}/${HOSTOS}/target/${TARGETARCH}/${TARGETOS}/${COMPILER}/base.Dockerfile \
  ${SWPATH}/gpgpu

# Remove the .dockerignore from //sw/gpgpu.
rm ${SWPATH}/gpgpu/.dockerignore

