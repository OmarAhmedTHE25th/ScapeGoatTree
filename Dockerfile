# Use a stable Ubuntu base
FROM ubuntu:22.04

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install essential build tools, Python, and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    python3-dev \
    && rm -rf /var/lib/apt/lists/*

# Install pybind11 for the Python bindings
RUN pip3 install pybind11

# Set the working directory in the container
WORKDIR /ScapeGoatProject

# Copy the entire project into the container
COPY . .

# Create a build directory
RUN mkdir build && cd build && \
    cmake .. && \
    cmake --build . --config Release

# Set the working directory to the build folder for execution
WORKDIR /ScapeGoatProject/build

# By default, run the TUI
CMD ["./TUI"]