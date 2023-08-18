sudo apt update

# List of packages to install
packages=(
    libtirpc-dev
    build-essential
    rpcbind
)

sudo apt install -y "${packages[@]}"
