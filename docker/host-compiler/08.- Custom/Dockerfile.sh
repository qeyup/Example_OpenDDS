#!/bin/bash


# Install miscellaneous usefull tools
apt update --fix-missing
apt upgrade -y
apt install -y  git \
                unzip \
                wget \
                curl \
                vim \
                bash-completion \
                tree \
                net-tools \
                cmake \
                rsync \
                qtcreator


# download
wget https://github.com/kepkin/dev-shell-essentials/archive/master.zip


# Install pyhon3 
apt install -y  python3 \
                python3-pip


# Install fuzzyfinder
git clone --depth 1 http://github.com/junegunn/fzf.git /home/.fzf
echo "" | /home/.fzf/install


# Install and configure zsh
apt install -y zsh
git clone http://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh
cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc
chsh -s /bin/zsh
sed -i s/"ZSH_THEME=.*"/"ZSH_THEME=bira"/g ~/.zshrc


# Install highlight
unzip master.zip -d /usr/local/
echo "source  /usr/local/dev-shell-essentials-master/dev-shell-essentials.sh" >> /etc/bash.bashrc
