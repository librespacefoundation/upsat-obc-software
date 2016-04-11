# /usr/bin/env sh
if [ ! -d Unity ]; then
  git clone https://github.com/ThrowTheSwitch/Unity.git
fi
if [ ! -d CMock ]; then
  git clone --recursive https://github.com/ThrowTheSwitch/CMock.git
fi
make mock-functions
