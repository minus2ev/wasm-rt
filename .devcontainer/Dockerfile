# Use the official Fedora Minimal base
FROM registry.fedoraproject.org/fedora-minimal:latest

# Install Clang, build tools, and VS Code requirements
# microdnf is the package manager for Fedora-minimal
RUN microdnf install -y \
    bash \
    clang \
    clang-tools-extra \
    lld \
    cmake \
    make \
    ninja-build \
    git \
    shadow-utils \
    tar \
    gzip \
    && microdnf clean all

# VS Code requires a non-root user for best experience
RUN useradd -m vscode
USER vscode