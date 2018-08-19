#!/bin/bash

ps aux | grep 'live-server' | grep -v 'grep' | awk '{print $2}' |  xargs -Ipid kill -9 pid
