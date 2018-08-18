#!/bin/bash
cd $(dirname "$0")/../..

WORKSPACE_PATH=$(pwd)

git clone git@github.com:cuttle-system/cuttle-parser.git
cuttle-parser/scripts/get-deps.sh

cd $WORKSPACE_PATH

git clone git@github.com:cuttle-system/cuttle-translator.git
cuttle-translator/scripts/get-deps.sh

cd $WORKSPACE_PATH

git clone git@github.com:cuttle-system/cuttle-test.git
