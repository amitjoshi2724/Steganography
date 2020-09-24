#!/bin/sh
node index.js > output.txt
# gunicorn server:app -b 127.0.0.1:$PORT -w=4