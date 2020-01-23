#!/bin/sh
protoc --cpp_out=./ *.proto
mkdir -p ../include/messages
mkdir -p ../src/messages
mv *.h ../include/messages/
mv *.cc ../src/messages/

