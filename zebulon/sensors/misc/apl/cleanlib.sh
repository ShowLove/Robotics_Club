#!/bin/bash

for I in "*lib/*.so" "*.mk" "*.tags" "*.session" "*/.debug" "*/.release"
do
	find ./ -wholename $I -exec rm -rv \{\} \;
done
