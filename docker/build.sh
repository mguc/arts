#!/bin/bash
set -e

DOCKER_USER=gucaninm
DOCKER_REPO=arts

HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $HERE

docker image build --no-cache -t $DOCKER_USER/$DOCKER_REPO .
# docker push $DOCKER_USER/$DOCKER_REPO
