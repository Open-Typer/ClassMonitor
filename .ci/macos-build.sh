#!/bin/bash

.ci/common/build.sh macos

macdeployqt ClassMonitor.app -dmg
