自用的、基于OpenWrt/LEDE/KOS，编译环境需要安装：
gcc, binutils, bzip2, flex, python, perl, make,find, grep, diff, unzip, gawk, getopt, subversion, libz-dev, libc
或者
sudo apt-get install asciidoc bash bc binutils bzip2 fastjar flex git-core gcc util-linux gawk libgtk2.0-dev intltool jikespg zlib1g-dev make genisoimage libncurses5-dev libssl-dev patch perl-modules rsync ruby sdcc unzip wget gettext xsltproc zlib1g-dev libboost1.55-dev libxml-parser-perl libusb-dev bin86 bcc sharutils openjdk-7-jdk subversion

在源码的根目录运行 "make package/symlinks" 自动下载扩展包,安装扩展包.

使用 "make menuconfig" 选择好配置后，如果您是第一次配置,建议使用"make download V=s"下载源码包，如果想多线程编译编译,请使用"make -j(线程数)",如果中间出错,请使用"make V=s"排除错误，建立工具链，编译内核和你选中的插件包。


编译笔记（部分）
#使用主目录
cd ~
#从GitHub拉取{项目}代码，放在{项目}目录下
git clone https://github.com/lede-project/source.git lede
git clone https://git.kos.org.cn/openkos/source.git kos
git clone https://github.com/openwrt/source.git openwrt

#下载apfree_wifidog源码
git clone https://github.com/liudf0716/apfree_wifidog.git apfree_wifidog

/home/20141009/apfree_wifidog/package/luci-app-apfree_wifidog
----->Makefile
        include ../../feeds/luci/luci.mk

修改默认主题：/kos/feeds/luci/collections/luci


#将apfree的packag复制到{项目}目录下
cp -r ~/apfree_wifidog/package/apfree_wifidog/ ~/lede/package/
cp -r ~/apfree_wifidog/package/luci-app-apfree_wifidog/ ~/lede/feeds/luci/applications
cp -r ~/apfree_wifidog/package/apfree_wifidog/ ~/kos/package/
cp -r ~/apfree_wifidog/package/luci-app-apfree_wifidog/ ~/kos/feeds/luci/applications
#如果你需要支持HTTPS，还需要安装libevent
cp -r ~/apfree_wifidog/package/libevent2/ ~/lede/package/libs/
#进入{项目}目录
cd lede/

#删除临时文件
rm -rf tmp/
#更新feeds
./scripts/feeds update -a
./scripts/feeds install -a
此外更换feeds：./scripts/feeds update -i  只更新feeds产生的index
#检测编译环境
make defconfig
make prereq
make menuconfig
make kernel_menuconfig
#显示磁盘用量
df –i /home
#编译参数设置
make -j1 V=s
make .config740nv3 -j4 V=s
#清理缓存
make clean
make dirclean
#单文件编译
make package/acctl/compile V=99
#解压缩文件
tar -zxvf cosbeta.tar
#tar.xz打包文件
tar xvJf
#切换到root权限和退出
sudo -i
exit
