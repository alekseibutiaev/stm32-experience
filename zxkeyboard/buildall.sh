#!/bin/bash

#set -x

build_keyboard() {
  cmakeparam="-DCMAKE_VERBOSE_MAKEFILE:STRING=ON -DCMAKE_TOOLCHAIN_FILE:STRING=tools_descriptor.cmake -DDUMP_ASM:STRING=OFF"
  cmakeparam=`echo "${cmakeparam} -DCMAKE_BUILD_TYPE:STRING=${1} -DMCU_TYPE:STRING=${2} -DUSB_POWER_HIGH:STRING=${3} -DBOARD_NUCLEO:STRING=${4}"`
  dir=`echo "build_${1}_${2}"`
  if [ ${3} == "ON" ] ; then
    dir=`echo ${dir}"_high"`
  else
    dir=`echo ${dir}"_low"`
  fi
  if [ ${4} == "ON" ] ; then
    dir=`echo ${dir}"_nucleo"`
  fi

  echo "dir: ${dir}"
  echo "params ${cmakeparam}"

  rm -rf ${dir}
  mkdir -v ${dir}
  pushd ${dir}

  cmake ../ ${cmakeparam} > log.txt 2>&1
  make -j4 >> log.txt 2>&1
  make Core/Src/usbh_platform.i >> log.txt 2>&1

  cat ./log.txt | grep -A2 "Memory region"

  popd
}

for type in Debug Release ; do
  for mcu in F105R8 F105RB F105RC F107RB F107RC ; do
    for act in ON OFF ; do
      if [ ${act} == "ON" ] ; then
        for nucleo in ON OFF ; do
          build_keyboard ${type} ${mcu} ${act} ${nucleo}
        done
      else
        build_keyboard ${type} ${mcu} ${act} "OFF"
      fi
    done
  done
done
