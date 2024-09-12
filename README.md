# Прошивка QMK

* [Прошивка для QMK](qmk/)


# Перепрошивка Sofle

1. Отключить вторую половину.
2. Замкнуть RESET на плате.
3. Компиляция и прошивка в QMK MSYS:

   Для левой стороны:
   ```
   qmk flash -kb sofle/rev2 -km kx -bl dfu-split-left
   ```

   Для правой стороны:
   ```
   qmk flash -kb sofle/rev2 -km kx -bl dfu-split-right
   ```

   HEX-прошивку можно залить через QMK Toolbox.

4. Повторить для второй половины.


# Прошивка для Vial

* [Скомпилированная HEX-прошивка для Vial](vial/)


# Раскладка клавиатуры для Windows

Единая раскладка клавиатуры с переключением между англиской и русской раскладками по Caps Lock. 
Коммпилируется программой Microsoft Keyboard Layout Creator (MSKLC).

* [Универсальная раскладка](msklc/)
