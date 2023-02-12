# 1. How to use raspi
- [1. How to use raspi](#1-how-to-use-raspi)
  - [1.1. 基本](#11-基本)
    - [1.1.1. Headlessセットアップ](#111-headlessセットアップ)
    - [1.1.2. winコマンドプロンプトからsshでつなぎに行く。](#112-winコマンドプロンプトからsshでつなぎに行く)
    - [1.1.3. raspiとのファイル移動](#113-raspiとのファイル移動)
    - [1.1.4. システム時刻](#114-システム時刻)
    - [1.1.5. vim設定ファイル](#115-vim設定ファイル)
    - [1.1.6. 起動時にプログラムを自動で実行する](#116-起動時にプログラムを自動で実行する)
    - [1.1.7. 起動後に一定時間おいて起動させる](#117-起動後に一定時間おいて起動させる)
  - [1.2. トラブルシュート](#12-トラブルシュート)
    - [1.2.1. wifiつながらない](#121-wifiつながらない)

## 1.1. 基本

### 1.1.1. [Headlessセットアップ](https://www.mikan-tech.net/entry/raspi-lite-headless-setup)

Raspbery pi os light 32bit等はGUIがないのでmicroSDのbootドライブに事前に以下のファイルを仕込む  
1. wifiの設定  
    'wpa_supplicant.conf'
    ```
    ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
    update_config=1
    country=JP
    network={
        ssid="<Name of your WiFi>"
        psk="<Password for your WiFi>"
    }
    ```
2. ssh有効化の設定  
    'ssh'という名前の空ファイル

### 1.1.2. winコマンドプロンプトからsshでつなぎに行く。
```
ssh ユーザ名@ホスト名 -p sshのポート  
ssh pi@raspberrypi -p 22  
```

### 1.1.3. raspiとのファイル移動
WinSCPを使うとできる

### 1.1.4. システム時刻
1. 'date'と打つ
2. 今の時刻と違う場合タイムゾーンが正しく設定されてないので'sudo raspi-config'と打って青画面を出す。
3. '5 Localisation Options'->'L2 Timezone'->'Asia'->'Tokyo'

### 1.1.5. vim設定ファイル
1. 'echo %HOME'と打って出てきたディレクトリに.vimrcファイルを作成する

### 1.1.6. [起動時にプログラムを自動で実行する](https://www.pc-koubou.jp/magazine/52061)

1. システムディレクトリ移動
    `cd /etc/systemd/system/'
2. サービスファイルを編集  
    'sudo vim test.service' [->ランレベル](https://tomosoft.jp/design/?p=11697)  
    ```ini
    [Unit]
    # ファイルの説明
    Description=Do test

    [Service]
    # 実行するプログラム、フルパスで設定
    ExecStart=/usr/bin/python3 /home/pi/Desktop/test.py

    [Install]
    # ランレベルを設定
    WantedBy=multi-user.target
    ```
3. サービスを再読み込み  
    'sudo systemctl daemon-reload'
4. サービスの動作状況確認  
    'sudo systemctl status test.service'
5. 実行できるか確認  
    'sudo systemctl start test.service'
6. 停止  
   'sudo systemctl stop test.service'
7. サービスの有効化  
    'sudo systemctl enable test.service'
8. サービスの無効化  
    'sudo systemctl disable test.service'  

### 1.1.7. 起動後に一定時間おいて起動させる  
1. タイマーファイルを作成  
    'sudo vim test.timer'
    ```ini
    [Unit]
    # ファイルの説明
    Description=Test timer

    [Timer]
    # ブート後、この時間後にこのタイマーを開始する
    # マイクロ秒 usec,us
    # ミリ秒 msec,ms
    # 秒 seconds,second,sec,s
    # 分 minutes,minute,min,m
    # 時 hours,hour,hr,h
    # 日 days,day,d
    # 週 weeks,week,w
    # 月 months,month,M
    # 年 years,year,y
    OnBootSec=3min
    # 繰り返し実行する時間の間隔
    OnUnitActiveSec=10s
    # 対象のサービス
    Unit=test.service

    [Install]
    # ランレベルを設定
    WantedBy=multi-user.target
    ```
2. タイマーの有効化  
'sudo systemctl start dumpvdl2.timer'  
'sudo systemctl enable dumpvdl2.timer'  
'sudo systemctl status dumpvdl2.timer'  



## 1.2. トラブルシュート

### 1.2.1. wifiつながらない  
https://zenn.dev/asataka/scraps/d45c78e7b400b4  

SOFTでblockedになっているのでunblockする。
```
root@pi4b:~# rfkill
ID TYPE      DEVICE      SOFT      HARD
 0 wlan      phy0     blocked unblocked
 1 bluetooth hci0   unblocked unblocked

root@pi4b:~# rfkill unblock 0
root@pi4b:~# rfkill
ID TYPE      DEVICE      SOFT      HARD
 0 wlan      phy0   unblocked unblocked
 1 bluetooth hci0   unblocked unblocked

root@pi4b:~# ip -br addr show dev wlan0
wlan0            UP             10.0.1.14/24
```

