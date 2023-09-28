#!/bin/bash

# This will stress the server with 10000 requests without waiting
for i in {1..1000}; do
    (
        # Generate a long Base64 encoded random string for the message
        msg=$(openssl rand -base64 10000)
        echo "Message $i: $msg"
    ) | nc 127.0.0.1 8080 &
done

# Wait for all background processes to finish
wait
