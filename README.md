== Перепрошивка Sofle ==

* Отключить вторую половину.
* Замкнуть RESET на плате.
* Компиляция и прошивка в QMK MSYS:

  Для левой стороны:
  ```
  qmk flash -kb sofle/rev2 -km kx -bl dfu-split-left
  ```

  Для правой стороны:
  ```
  qmk flash -kb sofle/rev2 -km kx -bl dfu-split-right
  ```

  HEX-прошивку можно залить через QMK Toolbox.

* Повторить для второй половины.


== Прошивка для Vial ==

* [Скомпилированная HEX-прошивка для Vial](vial/)
