#!/bin/bash
filename=$(cat $1 | sed 's/Daniel/\\e[31mDaniel\\e[0m/g')
echo -e "$filename"
