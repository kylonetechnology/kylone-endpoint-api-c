#!/bin/bash
#
# build_demo.sh
# Authors: Gokhan Poyraz <gokhan@kylone.com>
#
# Kylone Endpoint API, v2.0.81
# Copyright (c) 2018, Kylone Technology International Ltd.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
#

make="make"

function do_clean() {
   if [ -d "${1}/bin" -a -d "${1}/src" ]; then
      cd "${1}/src"
      ${make} clean
      cd "${1}"
      rm -rf "${1}/bin"
   fi
}

function err_check() {
  if test $? -ne 0; then
    exit
  fi
}

function do_compile() {
   if [ "${2}" == "" ]; then
      echo "Suppoted targets;"
      echo "  armv7"
      echo "  aarch64"
      echo "  linux-x86_64"
      echo "  macos"
      echo
      exit
   fi
   cd "${1}"
   TARGET="${2}" ${make}
   err_check
}

pw="`pwd`"
if [ "${1}" == "clean" ]; then
   do_clean "${pw}"
elif [ "${1}" == "make" ]; then
   do_clean "${pw}"
   do_compile "${pw}/src" "${2}"
else
   echo "> Usage ${0} <make <target> | clean>"
   echo
fi

