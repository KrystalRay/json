#!/bin/bash

ROOTDIR=$(dirname "$(dirname "$0")")

souffle -D$ROOTDIR -lclient -L$ROOTDIR/../src $ROOTDIR/test01.dl


