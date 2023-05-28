#!/bin/bash
function fib() {
	if [[ $1 -le 1 ]]; then
        return $1
    else
        fib $(($1-1))
        local fib1=$?
        fib $(($1-2))
        local fib2=$?
        return $(($fib1 + $fib2))
    fi
}

fib "$1"
echo "$?"
