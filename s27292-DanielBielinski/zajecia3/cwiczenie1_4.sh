#!/bin/bash
pliki=$(ls -p | grep -v / | wc -w)
if [[ $pliki -gt 5 ]]
then
    echo "Jest tu wiecej niz 5 plikow"
fi