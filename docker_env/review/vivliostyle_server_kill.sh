#!/bin/bash

ps aux | grep 'node_modules/http-server/bin/http-server' | grep -v 'grep' | awk '{print $2}' |  xargs -Ipid kill -9 pid
