# ARTS
ARTS stands for "Acoustic Ray Tracing Simulator" and is primarly designed to generate test data for Indoor Localization and Mapping Problems in a given 3D environment and for a set of receivers and sources.

## Requirements
### Docker
Probably the easiest and fastest way to get started is to install [Docker](https://docker.com). 
Just follow the instructions for your specific OS. 
After Docker has been installed just run the following command to pull the docker image and to start an interactive shell in a Docker container.

OSX and Linux:
```
docker/run.sh
```
Windows:
```
docker/run.bat
```

### Packages
If you are preferring to build the project locally, check out the [Dockerfile](docker/Dockerfile) for required packages.
Instead of building [CGAL](https://cgal.org) yourself you can install the package for your distribution. 
The version might differ, but the result should be the same. Examples:

**Ubuntu 18.04:**
```
sudo apt-get install libcgal-dev
```

## Building the project
In the project directory simply run:
```
cmake -DCMAKE_BUILD_TYPE=Release .
make
```
This will build two targets:
- `arts_cli`
- `arts_test`

## Running examples
The following example performs a simulation in a room with the dimensions 4m x 5m x 3m and one receiver and one source node sitting at position (0,0,0), which is the center of the room:
```
./arts_cli -e rsc/environment/4x5x3room.obj
```

If you run it natively you can add `-v` option to open the `CGAL Basic Viewer`:
```
./arts_cli -e rsc/environment/4x5x3room.obj -v
```

# Wish list
- Parallelization: CPU/GPU (if possible in Docker)
- Noise model
