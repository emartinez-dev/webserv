#!/bin/bash

# Create 100 clients that will send requests to the server
for i in {1..100}; do
    (
        echo "This is a message from client $i"
        sleep 1
    ) | nc 127.0.0.1 8080 &
    
    # Limit the number of concurrent connections to avoid overwhelming your machine
    if (( $i % 50 == 0 )); then
        wait
    fi
done

# Wait for all background processes to finish
wait
