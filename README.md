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
