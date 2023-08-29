#!/bin/bash
v="zxkeyboard_"`date +%Y%m%d_%H%M`
git tag ${v}
git push origin --tags
git tag --list | grep "zxkeyboard_" | tail -n3
