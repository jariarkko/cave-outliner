
# Installation

The software can be acquired from GitHub. Use these commands:

    git clone https://github.com/jariarkko/cave-outliner.git

After this, some prerequisite dependencies need to be installed,. On Ubuntu, install the following packeges:

    apt-get update
    apt-get install  g++ make pkg-config assimp-utils libassimp-dev imagemagick

Finally, compile the software as follows:

    cd cave-outliner
    make clean all

And then it is ready for use. The binary is ./cave-outliner

