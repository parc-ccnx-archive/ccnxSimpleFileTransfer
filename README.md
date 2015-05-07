CCN-Tutorial
=================

A simple file transfer server and client application for CCNx built on the Portal API.
It's intentionally skeletal.

[CCNx Tutorial main page](https://github.com/PARC/ccnx-tutorial)   
[CCNx.org](https://www.ccnx.org/)



This is the CCN Tutorial Demo, a very simple tutorial demonstrating
applications using the CCNx Portal API, the CCN Software Stack and the CCN
Metis Forwarder. It includes a set of programs, with source code, to serve
files and retrieve files.

The point of the tutorial is to demonstrate how to use the CCNx Portal API.
With this as an example, you should be able to understand how you could apply
the API to your own application.

After building, the demo consists of 2 programs:

* `tutorial_Server`: Serves files out of a directory.
* `tutorial_Client`: Lists and retrieves files from the server.

REQUIREMENTS
------------

The CCNx-Tutorial-Demo needs the Distillery CCNx distribution installed on the
system. Please download and install Distillery. [https://www.ccnx.org/download/] (https://www.ccnx.org/download/)

Obtaining CCNx Tutorial
-----------------------

You can obtain the CCNx Tutorial code by downloading it from [github] (https://github.com/PARC/ccnx-tutorial).


Building and Running
--------------------

To run the tutorial programs you will need the CCN forwarder (metis) running.
Assuming you've unpacked the Distillery tarball into the default location
(`/usr/local/ccnx/`), `/usr/local/`, Metis is installed with the rest of the CCNx
software in `/usr/local/ccnx/bin`.

To fully run the tutorial, you will need to run the `metis_daemon`, then the
`tutorial_Server` (to serve files) and then the `tutorial_Client` to access the
server.   It is recommended that you run the `metis_daemon`, `tutorial_Server`
and `tutorial_Client` in different windows.

Compiling the tutorial:

1. Go into the tutorial directory created when you cloned or unpacked the
   tutorial:   
   `$ cd ccnx-tutorial-<version>`

2. Configure the tutorial program:  
`$ ./configure --prefix=$HOME/ccnx`.  
The `--prefix=` argument specifies the destination directory if you run
 `make install`

3. Compile the tutorial, setting the `LD_RUN_PATH` for the compiled executables:  
`$ LD_RUN_PATH=/usr/local/ccnx/lib:/usr/local/parc/lib make`  
The `LD_RUN_PATH` sets the default place where the binaries will find the shared
 libraries. If you don't set the `LD_RUN_PATH` you will have to specify the
 location of the libraries via `LD_LIBRARY_PATH` when you run the executables.

4. At this point, the compiled binaries for `tutorial_Client` and the
`tutorial_Server` can be found in the `tutorial/src` directory.

5. Install the tutorial binaries to the specified prefix in the
configure step (eg `$HOME/ccn`). You will then be able to find the binaries in
the bin directory (eg `$HOME/ccn/bin`)
`make install`

6. Start the CCNx forwarder, `metis_daemon`:  
`$ /usr/local/ccnx/bin/metis_daemon &`

7. Running the tutorial_Server and tutorial_Client:
  Start the tutorial_Server, giving it a directory path as an argument.  
  `$HOME/ccnx/bin/tutorial_Server /path/to/a/directory/with/files/to/serve`

8.  In another window, run the tutorial_Client to retrieve the list of files
  available from the tutorial_Server. Do not run the tutorial_Client from the
  same directory you are serving files from.  
 ` $HOME/ccnx/bin/tutorial_Client list ` Will return a list of files from the tutorial_Server  
  Or, use the tutorial_Client to fetch a file from the tutorial_Server. 
  `$HOME/ccnx/bin/tutorial_Client fetch <filename>`    
  Will fetch the specified file

## Notes: ##

- The `tutorial_Client` and `tutorial_Server` automatically create keystore files in
  their working directory.

- If you run `tutorial_Client` on the same directory that the `tutorial_Server` is
  serving files from you will run into problems when you try to fetch a file.

- `tutorial_Client` and tutorial_Server require `metis_daemon` to be running.


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



