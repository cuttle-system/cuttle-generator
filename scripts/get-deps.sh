#!/usr/bin/env bash

cd $(dirname "$0")
SCRIPTS_PATH=$(pwd)
cd ../..
WORKSPACE_PATH=$(pwd)

for dep in $SCRIPTS_PATH/deps/*
do
    cd $WORKSPACE_PATH
    echo Getting dependency: $dep
    $dep
done