# Qt GraphViz libcgraph wrapper

The baseline library used in the new CUDA-centric Open|SpeedShop Graphical User Interface (GUI) which allows GraphViz DOT formatted data to be imported into
a Qt application by wrapping the GraphViz libcgraph library within the Qt Graphics View Framework.    The Graphviz libgvc library is used to compute the layout of the graph using the DOT rendering engine.

The graph structure can be created manually either by:

- constructing the individual graph canvas and then adding nodes and connecting edges
- constructing the graph, node and edges from an existing GraphViz DOT definition stored as a character array.


## Instructions

## Install GraphViz or Build from source

NOTE:  If GraphViz needs to be built from source, the latest stable snapshot tarball can be found here:

http://www.graphviz.org/pub/graphviz/stable/SOURCES/graphviz-2.40.1.tar.gz

For NASA HECC machines, the Graphviz development environment can be found in one of the 'pkgsrc' packages.  For example:

```
module load pkgsrc/2016Q2
```

Thus, the GRAPHVIZ_ROOT environment variable would be the following:

```
export GRAPHVIZ_ROOT=/nasa/pkgsrc/2016Q2
```

For Ubuntu 14.04 or 16.04 LTS systems, the Graphviz develop environment can be installed via:

```
sudo apt-get install libgraphviz-dev
```

## Build and Install the QtGraph library

Clone this repo and open the qmake project file named "QtGraph.pro" with Qt Creator or by running qmake manually.

Configure the INSTALL_ROOT and GRAPHVIZ_ROOT variables as desired.

The INSTALL_ROOT macro defines the installation directory for the QtGraph library header and library files in the following
directory structure:

```
INSTALL_ROOT/include/QtGraph
      |
      |     /lib64/$$QT_VERSION
```

The GRAPHVIZ_ROOT variable should be set to the installation directory of the Graphviz header and library files.

Once the build completes type "make install" to have the header and libraries files copied into the directory structure
show above.


## Build and Execute the simple QtGraph library example application

There is a simple example provided in the "examples" directory.  Open and build the qmake project file named "QtGraph-example.pro"
with Qt Creator or by running qmake manually.  Insure that the INSTALL_ROOT and GRAPHVIZ_ROOT variables are the same as in the QtGraph
library build step.

By default the example constructs the graph from a string (in memory).  However by uncommenting the DEFINE in the QtGraphExample.pro file
the example will show how to construct the graph by adding individual vertices and then specifying the edges between verticies.


```
# Uncomment below if manual graph construction example is desired
#DEFINES += MANUAL_GRAPH_CONSTRUCTION_EXAMPLE
```


## Contributions

Pull requests will be appreciated!  Contributions must be per the GNU Lesser General Public License (LGPL) used by this software.  Please following the same source-code formatting rules.
