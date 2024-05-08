## rx72n-envision-kit
RX72N Envision Kit bootloader/demos based on Amazon FreeRTOS

## RX72N Envision Kit product page (User's Manual, Schematics, How to buy, etc)
* en/ja: https://www.renesas.com/envision2

## rx72n-envision-kit wiki (Quick Start Guide, etc)
* en: [rx72n-envision-kit wiki (home)](https://github.com/renesas/rx72n-envision-kit/wiki)
* ja: [rx72n-envision-kit wiki (ホーム)](https://github.com/renesas/rx72n-envision-kit/wiki/%E3%83%9B%E3%83%BC%E3%83%A0)

## Board Introduction @ YouTube / ボード紹介動画 @ YouTube
* en:
    * [![](https://img.youtube.com/vi/tsOEiG4gNNE/0.jpg)](https://www.youtube.com/watch?v=tsOEiG4gNNE)
* ja:
    * [![](https://img.youtube.com/vi/ZC1WexzCJEo/0.jpg)](https://www.youtube.com/watch?v=ZC1WexzCJEo)

## Notice
* Please use git clone when you would try a debug with e2 studio.

## elements14: Renesas RX72N Envision Kit - Review
* https://www.element14.com/community/roadTestReviews/3641/l/renesas-rx72n-envision-kit-review

## Tracealyzer demo on RX72N Envision Kit
https://github.com/renesas/rx72n-envision-kit/assets/37968119/26790de7-03f8-4235-94a4-0f3afd66e6d4
  * 1) 0:00-0:08: Start: RX72N(includes Tracealyzer Recoder) connects to PC(Tracealyzer), PC can display the detailed RX72N internal status related on FreeRTOS.
  * 2) 0:08-0:10: AWS IoT Core connection: RX72N Envision Kit connected to AWS IoT Core. CPU usage (upper-center window) indicates 100% for TLS connection establishing to AWS IoT Core connection.
  * 3) 0:25-0:30: Change demo to Firmware Update from SD Card: Push "next" button to change demo display. 
  * 4) 0:30-0:48: Firmware Update from SD Card Demo: Push "update" button to start firmware update. Flash self-programing (Target flash memory size is 2MB) and SD card reading are occuring and these behaviour can be monitored through Tracealyzer.
