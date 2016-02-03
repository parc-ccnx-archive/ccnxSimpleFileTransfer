CCN-Tutorial
=================

A simple file transfer server and client application for CCNx built on the Portal API. 

[CCNx Tutorial main page](https://github.com/PARC/ccnx-tutorial)   
[CCNx.org](https://www.ccnx.org/)


This is the CCN Tutorial Demo, a very simple tutorial demonstrating
an application using the CCNx Portal API, the CCN Software Stack and the CCN Metis Forwarder.  It includes a set of programs, with source code, to serve
files and retrieve files. Files are transferred using a basic 'chunked' protocol.

The point of the tutorial is to demonstrate how to use the CCNx Portal API.
With this as an example, you should be able to understand how you could apply
the API to your own application.

After building, the demo consists of 2 programs:

* `tutorial_Server`: Serves files out of a directory.
* `tutorial_Client`: Lists and retrieves files from the server.

REQUIREMENTS
------------

The CCNx-Tutorial-Demo needs the Distillery CCNx distribution installed on the
system. Please install the [CCNx Distillery](  https://github.com/PARC/CCNx_Distillery) by downloading it from GitHub, [here]( https://github.com/PARC/CCNx_Distillery), and
building it according to the instructions there.



Building and Running
--------------------

To run the tutorial programs you will need a CCN forwarder (metis or athena) running.
We'll use metis for this example, but either would work.


Start metis by running 'metis_daemon --capacity 0', then the `tutorial_Server` (to serve files) 
and then the `tutorial_Client` to access the server.   It is recommended that you run 
the `metis_daemon`, `tutorial_Server` and `tutorial_Client` in different windows.

It is also recommended you run metis_daemon with the '--capacity 0' option to disable the cache
on the forwarder. This makes experimenting more predictable, as all Interests will make it
through to the tutorial_Server. 

Compiling the tutorial:

1. Go into the tutorial directory created when you cloned or unpacked the tutorial:   
   `$ cd ccnx-tutorial-<version>`

2. Set the CCNX_HOME environment variable to the location of your Distillery build. In zsh, for example,
it might look like this:
`export CCNX_HOME=/path/to/CCNx_Distillery/usr`
   

3. Compile the tutorial.
`$ make`  

    If 'make' has trouble finding libraries, such as libevent, you can also export LIBEVENT_HOME, like so: 

    * `export LIBEVENT_HOME=<value of DISTILLERY_EXTERN_DIR from 'make info' in your Distillery directory>`

    * e.g.  `export LIBEVENT_HOME=/usr/local/ccnx/dependencies/build`


4. At this point, the compiled binaries for `tutorial_Client` and the
`tutorial_Server` should be built.

5. Optionally install the tutorial binaries to the specified prefix in the
configure step (eg `$HOME/ccnx`). You will then be able to find the binaries in
the bin directory (eg `$HOME/ccnx/bin`)
`make install`

6. Start the CCNx forwarder, `metis_daemon`:  
`$HOME/ccnx/bin/metis_daemon --capacity 0&`

7. Running the tutorial_Server and tutorial_Client:
  Start the tutorial_Server, giving it a directory path as an argument.  
  `$HOME/ccnx/bin/tutorial_Server /path/to/a/directory/with/files/to/serve`

8.  In another window, run the tutorial_Client to retrieve the list of files
  available from the tutorial_Server. Do not run the tutorial_Client from the
  same directory you are serving files from.  
  `$HOME/ccnx/bin/tutorial_Client list ` Will return a list of files from the tutorial_Server  
  Or, use the tutorial_Client to fetch a file from the tutorial_Server. 
  `$HOME/ccnx/bin/tutorial_Client fetch <filename>`    
  Will fetch the specified file

## Notes: ##

- The `tutorial_Client` and `tutorial_Server` automatically create keystore files in
  their working directory.

- If you run `tutorial_Client` on the same directory that the `tutorial_Server` is
  serving files from you will run into problems when you try to fetch a file.

- `tutorial_Client` and tutorial_Server require `metis_daemon` to be running.

- The makefiles automatically set an LD_RUN_PATH variable so that you don't
  have to set it. They use the paths found by the configure script as default
  vaules.  If a different value is found in the environment then that will be
  used.  If you have no idea what this is about you can ignore this.

If you have any problems with the system, please discuss them on the developer 
mailing list:  `ccnx@ccnx.org`.  If the problem is not resolved via mailing list 
discussion, you can file tickets in our 
[issue tracker] (https://csltracker.parc.com/).


CONTACT
-------

For any questions please use the CCNx mailing list.  ccnx@ccnx.org


LICENSE
-------

This software is licensed under the BSD License.  See LICENSE File.


