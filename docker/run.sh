#!/bin/bash
set -e

docker run -it --rm --mount type=bind,source=$(pwd),target=/arts -w /arts gucaninm/arts
