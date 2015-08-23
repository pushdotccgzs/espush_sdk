#!/usr/bin/env bash

cd app/include
version=`date "+%Y%m%d"`-`git rev-parse --abbrev-ref HEAD`-`git rev-list HEAD -n 1|cut -c 1-8`
sed -i "s/[$]ESPUSH_SDKLIB_VERSION[$]/$version/" user_version.h


