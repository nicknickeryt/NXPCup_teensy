# NXPCup Teensy
This is a HAL and app recode based on [NXPCup](https://github.com/nicknickeryt/NXPCup/).\
It is not a port of the existing NXP_HAL/halina, but a new NXP_HAL that is based on it.

## Building
This app can be built using `west`, which is a build tool for Zephyr RTOS.\
In order not to store the whole Zephyr codebase in this repo, we use `west.yml` to automatically clone the proper Zephyr revision.

First, let's init west project:
```
west init -m https://github.com/nicknickeryt/NXPCup_teensy --mr wip/cpp-hal-rewrite NXPCup_teensy
```

Then automatically download Zephyr source code and necessary libs:
```
cd NXPCup_teensy/
west update
```

The above steps should only be done once and are unnecessary in subsequent builds.

Finall, let's build the app:
```
west build NXPCup_teensy/
```

## Flashing
You can flash the target board with:
```
west flash
```
inside the top-level project directory.
