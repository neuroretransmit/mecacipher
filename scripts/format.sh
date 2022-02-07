#!/bin/sh

clang-format -i $(find src/ -name '**.h') $(find src/ -name '**.cc')

