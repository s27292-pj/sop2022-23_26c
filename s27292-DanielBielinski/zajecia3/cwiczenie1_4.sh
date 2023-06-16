#!/bin/bash
pliki=$(ls -p | grep -cv /)
if [[ $pliki -gt 5 ]]
then
    echo "Jest tu wiecej niz 5 plikow"
fi