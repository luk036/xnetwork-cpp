FROM gitpod/workspace-full

USER root
# Install util tools.

RUN apt-get update \
 && apt-get install -y \
  apt-utils \
  aria2 \
# c++ stuff \
  gcc-10 \
  g++-10 \
  clang-13 \
  ccache \
  cmake-curses-gui \
  clang-format \
  clang-tidy \
  cppcheck \
  doctest-dev \
  kcachegrind-converters \
  kcachegrind \
  lcov \
  libbenchmark-dev \
  libboost-dev \
  libfmt-dev \
#  libjsoncpp-dev \
#  libopenblas-dev \
  librange-v3-dev \
  libspdlog-dev \
  ninja-build \
# utilities (not ripgrep, gh) \
  asciinema \
  bat \
  byobu \
  curl \
  elinks \
  fd-find \
  fish \
  mdp \
  ncdu \
  neofetch \
  patat \
  pkg-config \
  ranger \
  w3m \
# just for fun (not cmatrix) \
  cowsay \
  figlet \
  fortune \
  toilet \
  tty-clock

RUN mkdir -p /workspace/data \
    && chown -R gitpod:gitpod /workspace/data
  
RUN mkdir /home/gitpod/.conda
# Install conda
RUN wget --quiet https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O ~/miniconda.sh && \
    /bin/bash ~/miniconda.sh -b -p /opt/conda && \
    rm ~/miniconda.sh && \
    ln -s /opt/conda/etc/profile.d/conda.sh /etc/profile.d/conda.sh && \
    echo ". /opt/conda/etc/profile.d/conda.sh" >> ~/.bashrc && \
    echo "conda activate base" >> ~/.bashrc
    
RUN /opt/conda/bin/conda config --set always_yes yes --set changeps1 no \
    && /opt/conda/bin/conda update -q conda \
    && /opt/conda/bin/conda info -a

RUN /opt/conda/bin/conda install -y -c conda-forge \
    cmake-format \
    pandoc-crossref \
    pandoc

RUN chown -R gitpod:gitpod /opt/conda \
    && chmod -R 777 /opt/conda \
    && chown -R gitpod:gitpod /home/gitpod/.conda \
    && chmod -R 777 /home/gitpod/.conda

RUN apt-get clean && rm -rf /var/cache/apt/* && rm -rf /var/lib/apt/lists/* && rm -rf /tmp/*
