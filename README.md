# Qt GraphViz libcgraph wrapper

The baseline library used in the new CUDA-centric Open|SpeedShop Graphical User Interface (GUI) which allows GraphViz DOT formatted data to be imported into
a Qt application by wrapping the GraphViz libcgraph library within the Qt Graphics View Framework.    The Graphviz libgvc library is used to compute the layout of the graph using the DOT rendering engine.

The graph structure can be created manually either by:

- constructing the individual graph canvas and then adding nodes and connecting edges
- constructing the graph, node and edges from an existing GraphViz DOT definition stored as a character array.


## Instructions

## Build and Install the QtGraph library

Clone this repo and open the qmake project file named "QtGraph.pro" with Qt Creator or by running qmake manually.  Configure the INSTALL_ROOT macro as desired.

The INSTALL_ROOT macro defines the installation directory for the QtGraph library header and library files in the following
directory structure:

```
INSTALL_ROOT/include/QtGraph
      |
      |     /lib64/$$QT_VERSION
```

Once the build completes type "make install" to have the header and libraries files copied into the directory structure
show above.

## Build and Execute the simple QtGraph library example application

There is a simple example provided in the "examples" directory.  Open and build the qmake project file named "QtGraph-example.pro"
with Qt Creator or by running qmake manually.  Insure that the INSTALL_ROOT macro is the same as in the QtGraph library build step.


## Contributions

Pull requests are appreciated!  Contributions must be per the GNU Lesser General Public License (LGPL) used by this software.  Please following the same source-code formatting rules.
