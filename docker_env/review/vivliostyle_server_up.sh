#!/bin/bash

VIVLIOSTYLE_DIR=./docker_env/review/vivliostyle
VIVLIOSTYLE_VERSION=2017.6
VIVLIOSTYLE_ZIP=vivliostyle-js-${VIVLIOSTYLE_VERSION}.zip
VIVLIOSTYLE_UNZIP_DIR=vivliostyle-js-${VIVLIOSTYLE_VERSION}
VIVLIOSTYLE_BIN=${VIVLIOSTYLE_DIR}/${VIVLIOSTYLE_UNZIP_DIR}/start-webserver

${VIVLIOSTYLE_BIN}