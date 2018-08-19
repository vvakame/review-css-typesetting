#!/bin/bash

VIVLIOSTYLE_DIR=./docker_env/review/vivliostyle
VIVLIOSTYLE_VERSION=2017.6
VIVLIOSTYLE_ZIP=vivliostyle-js-${VIVLIOSTYLE_VERSION}.zip
VIVLIOSTYLE_UNZIP_DIR=vivliostyle-js-${VIVLIOSTYLE_VERSION}
VIVLIOSTYLE_BIN=${VIVLIOSTYLE_DIR}/${VIVLIOSTYLE_UNZIP_DIR}/start-webserver

if [ ! -f ${VIVLIOSTYLE_BIN} ]; then
mkdir -p ${VIVLIOSTYLE_DIR}
wget https://github.com/vivliostyle/vivliostyle.js/releases/download/${VIVLIOSTYLE_VERSION}/${VIVLIOSTYLE_ZIP} -O ${VIVLIOSTYLE_DIR}/${VIVLIOSTYLE_ZIP}
unzip -d ${VIVLIOSTYLE_DIR} -o ${VIVLIOSTYLE_DIR}/${VIVLIOSTYLE_ZIP}
else
echo "vivliostyle binary is already exests."
fi
