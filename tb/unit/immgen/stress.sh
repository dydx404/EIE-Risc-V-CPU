#!/bin/bash

for i in {1..1000}; do
    echo "Run #$i"
    ./obj_dir/VExtend || { echo "Failed on run $i"; exit 1; }
done

echo "All runs passed."
