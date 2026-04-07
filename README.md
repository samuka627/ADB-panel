# ADB-panel

A small panel written in C that executes some ADB
commands. You can use this panel in Termux, in
conjunction with Wireless Debugging, to execute
ADB commands.

### How to Pair:
1. Access your phone's **Developer Options**.
2. Search for **Wireless Debugging** and enable it.
3. Click on **Pair device with pairing code**.
4. In Termux, type: `adb pair [IP_ADDRESS]:[PORT]`
5. Enter the **Pairing Code** when prompted.
6. Then type: `adb connect [IP_ADDRESS]:[PORT]` (using the address from the main Wireless Debugging screen).
