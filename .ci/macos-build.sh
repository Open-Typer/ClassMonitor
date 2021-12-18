#!/bin/bash

.ci/common/build.sh macos

cd ClassMonitor.app
zip -r ../Open-Typer-ClassMonitor-mac.zip .
