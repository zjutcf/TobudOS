﻿# 一、智能灯介绍

传统的家用照明是通过手动拨动物理开关来接通、断开电源进而控制灯泡的亮灭。智能照明则是通过给灯泡增加网络模块这一物理特性，赋予了灯泡连接网络的能力。进而允许用户通过 APP、网页、智能接入设备等方式控制灯泡的开关。

本项目制作的智能灯可以远程控制开关，调节预设挡位亮度，可以实时检测所处环境的光照强度并在 OLED 屏幕上显示具体数值。同时还可以将数据实时上报到腾讯云物联网开发平台 IoT Explorer，用户可以在“腾讯连连”小程序实时查看数据并控制灯泡。

本项目所需要的物品清单有：

- TOS_EVB_G0 开发板 x 1
- 智慧路灯扩展板 E53_SC1

![](https://main.qcloudimg.com/raw/a1ace5677ba77d4e5ea42983bd9b0d74.png)

**成品组装图（安装各接插件时，请务必参考成品图，不要弄错方向，否则可能烧坏 MCU）**
![](https://main.qcloudimg.com/raw/503ff0e4e9df234d3d524d0a4d7ed083.jpg)
好啦~接下来开始我们的制作！

# 二、TOS_EVB_G0 开发板

## 1. 开发板特性

TOS_EVB_G0 开发板是由腾讯 TencentOS-tiny 团队设计的一款物联网开发板，板载资源如下：

![](https://main.qcloudimg.com/raw/45564fdbaf9f5d00ddfda2927c9e6555.jpg)

- 主控芯片采用 STM32G070RB，Flash 空间仅有 128KB、RAM 空间仅有 20KB；
- 板载腾讯云定制固件版 ESP8266 WIFI 模组；
- 板载 E53 传感器标准接口，方便连接各种 E53 传感器；
- 板载 0.91'OLED 显示屏幕；
- 板载 8MB SPI Flash，可用于固件升级；
- 板载 CH340 转串口连接，可以使用一根 USB 线连接至电脑，查看串口日志；

下面是每部分详细的说明。

## 2. 开发板硬件电路详解

### 2.1. 最小系统电路

最小系统部分由电源电路、时钟电路、Boot 电路、复位电路、下载电路组成：
![](https://img-blog.csdnimg.cn/2020091215453594.png)

### 2.2. WIFI 电路

WIFI 部分包括 ESP8266 WIFI 模组供电电路、串口通信电路：
![](https://img-blog.csdnimg.cn/20200912154639745.png)

### 2.3. E53 扩展接口电路

E53 扩展接口供外部传感器连接，包括：一个串口、一个 SPI 接口、一个 IIC 接口、四个普通 GPIO 口、一个 ADC 模拟采集接口，一个 DAC 模拟输出接口，如图：
![](https://img-blog.csdnimg.cn/20200912154756810.png)

### 2.4. USB 转串口电路

USB 转串口电路将串口信息转换为 USB 口信号，使得开发板可以直接通过 USB 线连接至 PC，在 PC 上使用串口助手查看日志：
![](https://img-blog.csdnimg.cn/20200912155018316.png)

### 2.5. OLED 显示屏幕电路

OLED 屏幕部分包括 OLED 供电部分和通信部分：
![](https://img-blog.csdnimg.cn/20200912155131688.png)

### 2.6. 连接 Micor USB 线

![](https://main.qcloudimg.com/raw/a00746e285381da66af9f9eb4b556078.jpg)

### 2.7. 连接 ST-Link 下载器

开发板 3v3---> STlink 3.3V
开发板 DIO---> STlink SWDIO
开发板 CLK---> STlink SWCLK
开发板 GND---> STlink GND
**请大家参考下图进行连接，千万不要接反接错，以免烧掉 MCU**
![](https://main.qcloudimg.com/raw/4ab5937dd0221e4ad1b0109b2f028aa1.jpg)

## 3. 开发板软件环境准备

### 3.1 MDK 软件介绍

MDK 即 RealView MDK 或 MDK-ARM（Microcontroller Development kit），是 ARM 公司收购 Keil 公司以后，基于 uVision 界面推出的针对 ARM7、ARM9、Cortex-M0、Cortex-M1、Cortex-M2、Cortex-M3、Cortex-R4 等 ARM 处理器的嵌入式软件开发工具。MDK-ARM 集成了业内最领先的技术，包括 uVision4 集成开发环境与 RealView 编译器 RVCT。支持 ARM7、ARM9 和最新的Cortex-M3/M1/M0 核处理器，自动配置启动代码，集成 Flash 烧写模块，强大的 Simulation 设备模拟，性能分析等功能，与 ARM 之前的工具包 ADS 等相比，RealView 编译器的最新版本可将性能改善超过 20%。

Keil 公司开发的 ARM 开发工具 MDK，是用来开发基于 ARM 核的系列微控制器的嵌入式应用程序。它适合不同层次的开发者使用，包括专业的应用程序开发工程师和嵌入式软件开发的入门者。MDK 包含了工业标准的 Keil C 编译器、宏汇编器、调试器、实时内核等组件，支持所有基于 ARM 的设备，能帮助工程师按照计划完成项目。

### 3.2 MDK 安装

登录 MDK 官网，下载 MDK5 软件，下载地址：
http://www2.keil.com/mdk5 或者 https://www.keil.com/download/product/

下载的版本最好在 5.24 以上，本开发教程以 5.24 版本为例，双击 MDK524 应用程序文件，点击 next>>。
![](https://img-blog.csdnimg.cn/20200912160353792.png)

打上 I agree 前面的勾勾，即是同意一些安装协议。点击 next>>。

![](https://img-blog.csdnimg.cn/20200912160415757.png)

选择安装路径，可以默认也可以安装在我们自己建立的文件夹下。点击 next>>。

![](https://img-blog.csdnimg.cn/20200912160425324.png)

这里填写的是我们的一些信息，填写完整后，继续 next>>。

![](https://img-blog.csdnimg.cn/20200912160432240.png)

然后等待安装完成即可。

![](https://img-blog.csdnimg.cn/20200912160448600.png)

安装完成，点击 Finish。
![](https://img-blog.csdnimg.cn/20200912160455488.png)

然后会跳出来这个界面，这个我们后面再讲，先点 OK，把弹框都叉掉。
![](https://img-blog.csdnimg.cn/20200912160508347.png)

激活 MDK，导入 License，激活 MDK 后便可使用了。

![](https://img-blog.csdnimg.cn/20200912160520293.png)
![](https://img-blog.csdnimg.cn/202009121605276.png)
特别提示：一定要输入 License 激活 MDK 软件，建议购买正版 License。

### 3.3 MDK 安装

安装完 MDK 后，我们需要安装开发套件中单片机型号对应的 Pack。
**安装方式一** 登录官网：http://www.keil.com/dd2/pack/

下载 Keil.STM32G0xx_DFP.x.x.x.pack 后安装。

**安装方式二** MDK 软件上在线安装

打开软件，在导航栏打开 Pack 安装界面，然后选择 ok 选项。

![](https://img-blog.csdnimg.cn/20200912160604878.png)

![](https://img-blog.csdnimg.cn/20200912160612930.png)

进入在线安装界面，选着 STM32G0XX Pack，点击 Install 进行安装。

![](https://img-blog.csdnimg.cn/20200912160628442.png)
至此，我们开发板的单片机程序开发环境已经搭建完毕，重启 MDK 软件就可以使用了。

### 3.4 ST-Link 驱动安装

前面讲了开发板单片机程序的开发环境的搭建，但是为了将程序烧录到开发板中我们还需要使用仿真器。我们这套开发板选用 ST 公司的 ST-Link V2 仿真器进行开发板程序的烧写和仿真，下面介绍 ST-Link 驱动的安装及环境搭建。
在 ST 官网下载 ST-Link 驱动，
https://www.st.com/content/st_com/zh/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-utilities/stsw-link009.html

（驱动有 2 种：32 位电脑系统安装“dpinst_x86”、64 位电脑系统安装“dpinst_amd64”）。

![](https://img-blog.csdnimg.cn/20200912160942366.png#pic_center)

运行对应的驱动，安装 ST-Link V2 驱动程序。安装路径尽量保持默认路径。

![](https://img-blog.csdnimg.cn/20200912160951881.png)
安装完成后，将 ST-Link 通过 USB 接口连入电脑。打开“设备管理器”。若看到如下图所示，表示驱动安装成功。
![](https://img-blog.csdnimg.cn/20200912163834675.png)

这里提醒 2 点：
1， 各种 windows 版本设备名称和所在设备管理器栏目可能不一样，例如 WIN10 插上 STLINK 后显示的是 STM32 STLINK。
2， 如果设备名称旁边显示的是黄色的叹号，请直接点击设备名称，然后在弹出的界面点击更新设备驱动
至此，ST-Link 驱动已经安装完成。接下来大家只需要在 MDK 工程里面配置一下 ST-Link 即可。

### 3.5 CH340 串口驱动安装

互联网搜索下载 CH340 串口芯片的驱动
安装方法：打开驱动安装程序点击安装即可。

![](https://img-blog.csdnimg.cn/20200912161124298.png#pic_center)

![](https://img-blog.csdnimg.cn/20200912161131269.png)

注：若安装失败，请先点击卸载，后点击安装。

### 3.7 串口调试助手的安装与使用

工具下载：http://www.daxia.com/download/sscom.rar

安装方法：串口调试助手 sscom5.13.1 是免安装的，解压出压缩包即可直接使用。
![](https://img-blog.csdnimg.cn/20200912161144942.png)
根据 PC 和终端之间的连接，选择正确的串行端口。
打开电脑的设备管理器，在端口列表可以看到 PC 与开发板连接的端口号。
![](https://img-blog.csdnimg.cn/20200912163536203.png)

我这里显示的是 COM26，所以要在 sscom 工具中选择 COM26，开发板程序波特率设置为 115200，所以我在 sscom 串口工具中选择 115200 波特率。

# 三、烧写腾讯云定制固件

## 1. 下载固件

腾讯云 IoT AT 指令是一套针对使用通讯模组（2G/4G/NB/WIFI）接入腾讯云物联平台的定制 AT 指令集，如果通讯模组实现了该指令集，则设备接入和通讯更为简单，所需代码量更少。ESP8266 作为 IoT 领域使用最广泛的一款 WiFi 芯片/模组，腾讯云在其通用 AT 指令基础上，增加了腾讯云 IoT AT 指令集，形成一个定制的模组固件 QCloud_IoT_AT_ESP8266。

下载链接：[https://github.com/tencentyun/qcloud-iot-esp-wifi](https://github.com/tencentyun/qcloud-iot-esp-wifi)

使用 git 下载的命令如下：

```bash
git clone https://github.com/tencentyun/qcloud-iot-esp-wifi.git
```

下载之后即可在图中圈出的文件夹中看到定制固件：
![](https://img-blog.csdnimg.cn/20200912164445221.png#pic_center)

## 2. 下载烧写工具

使用乐鑫官方下载工具 ESPFlashDownloadTool，下载链接如下：

[https://www.espressif.com/zh-hans/support/download/other-tools](https://www.espressif.com/zh-hans/support/download/other-tools)

点击下载 Flash 下载工具（ESP8266 & ESP32 & ESP32-S2）：
![](https://img-blog.csdnimg.cn/20200912170728154.png#pic_center)
下载之后解压即可。

## 3. 固件说明

腾讯云 IoT 定制的 AT 模组固件 QCloud_IoT_AT_ESP8266，适用于所有 FLASH 大小为 2MB 或者 2MB 以上的 ESP8266 模组。

AT 串口使用 UART0，默认的 Tx 为 GPIO1，Rx 为 GPIO3。但因为 ESP8266 的 UART0 默认会在上电启动期间输出一些打印，如果打印信息影响设备功能，可在上电期间将 U0TXD(GPIO1)、U0RXD(GPIO3) 分别与 U0RTS (GPIO15)，U0CTS(GPIO13) 进行交换，以屏蔽打印。因此提供两个版本的固件：

- 名称包含 UART_1_3 的固件串口使用的 Tx 为 GPIO1，Rx 为 GPIO3。
- 名称包含 UART_15_13 的固件串口使用的 Tx 为 GPIO15，Rx 为 GPIO13。

进入 QCloud_IoT_AT_ESP8266_FW，本文中我们使用第一个固件：
![](https://img-blog.csdnimg.cn/20200912164714338.png#pic_center)

## 4. 烧写固件

### 4.1. 切换 boot 选择端子和串口选择端子

① 将开发板上的 ESP8266 启动引脚配置端子切换到低电平；

② 将开发板上的串口选择端子置于中间，使 ESP8266 直接与 PC 串口连接：

方法参考下图：
![](https://main.qcloudimg.com/raw/733a1c7406bcc2621e751ca07f0be0f7.jpg)

### 4.2. 运行 Flash 烧写工具

双击工具，运行：
![](https://img-blog.csdnimg.cn/2020091217093379.png)
选择开发者模式：
![](https://img-blog.csdnimg.cn/20200912171110420.png#pic_center)
选择 ESP8266 下载工具：
![](https://img-blog.csdnimg.cn/20200912171139314.png#pic_center)

### 4.3. 开始下载

按照图中进行操作：
![](https://img-blog.csdnimg.cn/20200912171454660.png)
点击下载之后，按下开发板上的复位键，Flash 下载工具开始下载：
![](https://img-blog.csdnimg.cn/20200912171547158.png)
若没有显示，请检查接线端子是否连接正确。

### 4.4. 下载完成

下载完成之后如图：
![](https://img-blog.csdnimg.cn/20200912171748705.png)
关闭工具，将 boot 选择引脚拔下，将串口选择端子恢复默认。

**注意：完成固件下载后，一定切记将跳线端子恢复到默认状态，默认端子怎么短接可以参考前面完整的组装成品图，否则 wifi 将没法工作，MCU 也没法控制 wifi。**

# 四、基于 TencentOS tiny 接入腾讯云 IoT Explorer 指引

基于 TencentOS Tiny EVB G0 要完成腾讯云 IoT Explorer 对接，需要完成两个部分的工作。
一是：腾讯云 IoT explorer 上完成项目、产品、设备创建、参数配置
二是：基于 TencentOS Tiny 完成应用开发，向腾讯云上报业务数据。

# 1. 云端操作步骤

## 1.1. 新建项目

登录[腾讯云物联网开发平台](https://cloud.tencent.com/product/iotexplorer)，点击新建项目，填写项目名称和简介：

![](https://main.qcloudimg.com/raw/e8ec3f87b580328d6ef38a64d47ec90b.png)

## 2.2. 新建产品

点击项目名称进入到该项目中，点击新建产品：
![](https://main.qcloudimg.com/raw/fe74a6729cda53a4eab6d82347415e08.png)
产品新建成功后，可在产品列表页查看到：
![](https://main.qcloudimg.com/raw/4f4e045f7d8daebbe7050fcccfdf788f.png)

## 2.3. 数据模板

进入产品，点击【数据模板】，点击【导入 JSON】，导入下面的 JSON 代码：

```json
{
  "version": "1.0",
  "properties": [
    {
      "id": "power_switch",
      "name": "电灯开关",
      "desc": "控制电灯开灭",
      "required": true,
      "mode": "rw",
      "define": {
        "type": "bool",
        "mapping": {
          "0": "关",
          "1": "开"
        }
      }
    },
    {
      "id": "lx",
      "name": "光照强度",
      "desc": "光照强度",
      "mode": "r",
      "define": {
        "type": "int",
        "min": "0",
        "max": "20000",
        "start": "0",
        "step": "1",
        "unit": "lx"
      },
      "required": false
    },
    {
      "id": "brightness_level",
      "name": "亮度等级",
      "desc": "亮度等级",
      "mode": "rw",
      "define": {
        "type": "enum",
        "mapping": {
          "0": "LOW",
          "1": "MID",
          "2": "HIGH"
        }
      },
      "required": false
    }
  ],
  "events": [],
  "actions": [],
  "profile": {
    "ProductId": "3P9346G7NL",
    "CategoryId": "539"
  }
}
```

导入之后自动根据 json 文件创建的属性如下：
![](https://main.qcloudimg.com/raw/79499fc777577423d0976f9f4b616e75.png)

## 2.4. 创建设备

点击【设备调试】，进入后点击【新建设备】，创建真实设备：
![](https://main.qcloudimg.com/raw/7361a8d69c552ae00931cb5a4ce28254.png)

创建成功之后进入设备，查看到产品 ID、设备名称、设备秘钥：
![](https://main.qcloudimg.com/raw/c90378470875a565a1b3d62941941159.png)

## 2. 基于 TencentOS Tiny 完成设备侧应用开发

进入 < TencentOS-tiny\board\TencentOS_tiny_EVB_G0\KEIL\\mqtt_iot_explorer_e53_light> 目录，打开 TencentOS_tiny.uvprojx 工程。

### 2.1. 修改 WIFI 接入信息

打开`mqtt_iot_explorer_e53_light.c`文件编辑，修改要接入 WIFI 的名称和密码（建议使用手机热点，不要有特殊字符）：
![](https://main.qcloudimg.com/raw/507248c95ae0ebbb3eb0378e95ec6308.png)

### 2.2. 修改云端对接信息

同样在此文件内，修改云端配置信息，使用之前在云端查看的信息替换：
![](https://main.qcloudimg.com/raw/51478f697c97b8ffd8311abbdefb5c92.png)

### 2.3. 编译工程

点击编译按钮，编译整个工程：
![](https://main.qcloudimg.com/raw/2d3ac56fefd1d6cfa075f3ca121ad93d.png)

### 2.4. 下载程序

点击进入工程配置界面按纽，进入工程配置界面：
![](https://main.qcloudimg.com/raw/e896fc448df7635062006dc8d5a8e4aa.png)
选择 Debug 选项，进入仿真器设置界面。
![](https://img-blog.csdnimg.cn/20200912163019926.png)

下拉仿真器选择列表，选着 ST-Link Debugger 并勾选右侧 Use，点击 Settings 进入 ST-Link V2 仿真器配置界面。
![](https://img-blog.csdnimg.cn/20200912163044635.png)

开发板设计的程序烧录方式为 SW，此处 Unit 选择 ST-Link/V2，且 Port 选择 SW，并确认右侧框内是否检测出 SW 设备，点击<确认>保存配置。
![](https://img-blog.csdnimg.cn/20200912163441583.png)
点击确定之后，回到主界面，点击下载按钮：
![](https://main.qcloudimg.com/raw/55a686de1ebf10e101386c62def3dc30.png)

### 2.5. 使用串口助手查看日志

运行 sscom 软件，打开电脑上开发板对应的串口，比如我选择 COM5，点击【打开串口】：
![](https://main.qcloudimg.com/raw/27fda47404e3e6001b1aee4fa20344f5.png)
按下开发板上的复位键，程序开始运行，即可在串口助手中看到系统运行打印的日志：
![](https://main.qcloudimg.com/raw/a05e4b4a46f086451fe9c8fadbc7fd67.png)

## 3. 云端平台查看设备数据

回到腾讯云物联网开发平台，可以看到设备状态变为【在线】：
![](https://main.qcloudimg.com/raw/6f9d9041aa12013f55ad0c7e548d81ad.png)
点击【设备日志】一栏，可以看到设备上报的光照强度和当前灯光级别数值：
![](https://main.qcloudimg.com/raw/0e4088d1b122cc5f302998cc087adfaa.png)
点击【在线调试】一栏，修改属性【期望值】后点击【发送】，可以对灯泡进行远程控制。
![](https://main.qcloudimg.com/raw/a77085bdab5a548a0de1edaa87bd001a.png)

# 五、腾讯连连小程序操作步骤

## 1. 添加家庭

手机端在【微信】搜索【腾讯连连】小程序，首次使用需要进入后点击【我的】->【家庭管理】，添加一个你喜欢的名称即可。

## 2. 添加调试设备

返回【首页】，点击右上角“加号”图标：
![](https://img-blog.csdnimg.cn/20200912192517415.png)

进入后点击右上角扫码图标：
![](https://img-blog.csdnimg.cn/20200912192742454.png)

在腾讯云物联网开发平台进入【设备调试】，点击对应设备后的【二维码】：
![](https://main.qcloudimg.com/raw/47708ce94ce1b7fd59d1effa168d3dfb.png)

腾讯连连扫描此二维码即可成功添加设备，添加成功之后如图：
![](https://main.qcloudimg.com/raw/08fdb88f56f524734a3916b8c6fe37e7.png)

点击此设备即可实时查看数据，并下发控制指令：
![](https://main.qcloudimg.com/raw/4db3af9e6fedc12a158a2a5cb58be2c8.png)

## 3. 修改腾讯连连显示面板

进入【交互开发】，点击【面板配置】一栏：
![](https://img-blog.csdnimg.cn/20200831165713961.png)
在此页面中可以自己根据喜好配置小程序显示面板，如图：
![](https://main.qcloudimg.com/raw/e63ed23e5053852c46e8f2ef12b1ad31.png)

保存之后，在腾讯连连小程序删除设备，重新扫描绑定设备即可生效。
