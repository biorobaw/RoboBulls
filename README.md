This is a branch from the Robobulls repository that implements
neural networks for determining shooting probabilities and DDPG for reinforcement learning
for low level move-to-ball and shooting behaviors.

[Shot predictor](https://github.com/justinandrewrodney/CanShootTrainer)

[Deeplearning trainer](https://github.com/justinandrewrodney/SSL_DDPG_rSoccer)

You will need libTensorflow and LibTorch(PyTorch C++) to run this project and update
their paths in the .pro file.

I had luck with the PyTorch precompiled binaries located here(https://download.pytorch.org/libtorch/cu102/libtorch-win-shared-with-deps-debug-1.10.1%2Bcu102.zip) 
I encountered issus when compiling LibTorch from source.

libTensorflow binaries can be found here: https://www.tensorflow.org/install/lang_c

robobulls2
==========

Narges super version of the new architecture for Williams robobulls

Documentation
-------------
See **docs/Robobulls Documentation.docx**

Installation
------------
See sections "Required Software" and "Compiling the code" in documentation file **docs/Robobulls Documentation.docx**
For instractions on how to install grSim (required softwre) on windows, see file **docs/Install grSim on windows.docx**

