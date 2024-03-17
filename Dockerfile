FROM ubuntu

WORKDIR /root/workspace

# Install linters and formatters.
RUN apt update && \
apt install -y gcc ccls clangd clang-tidy \
cppcheck cpplint flawfinder astyle clang-format uncrustify

# Install Git and clone dotfiles.
RUN apt update && apt install -y git sudo && \
git clone https://github.com/G-Ohara/dotfiles.git /root/dotfiles && \
cd /root/dotfiles && ./install_links.sh

# Install Neovim.
RUN cd /root/dotfiles/scripts/apps/ && ./neovim.sh
