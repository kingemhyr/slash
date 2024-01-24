#!/bin/env sh

if [ ! -d build ]; then
  mkdir build
fi

odin build source -out=build/slash -show-timings -debug
