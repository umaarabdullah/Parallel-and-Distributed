sudo apt update

# List of packages to install
packages=(
    libtirpc-dev
    build-essestials
    rpcbind
)

sudo apt install -y "${packages[@]}"
