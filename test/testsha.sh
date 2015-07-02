#!/bin/bash
echo SHA-2 Test Script

for i in 224 256 384 512; do
	for j in $( ls | grep '^vector[0-9]$' ); do
		../mysha.out $i $j > tmphash
		#../mysha.out $i $j > $j.sha$i
		if diff tmphash $j.sha$i >/dev/null 2>/dev/null; then
			echo \'$j\' passed SHA-$i.
		else
			echo \'$j\' did not pass with SHA-$i.
		fi
		rm tmphash >/dev/null 2>/dev/null
	done
done
