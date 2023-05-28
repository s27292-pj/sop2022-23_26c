#!/bin/bash
name=$0
if [[ ! "$0" =~ *.sh ]]
then
	mv "$0" "${0}.sh"
else
	echo "jest to plik .sh"
fi
