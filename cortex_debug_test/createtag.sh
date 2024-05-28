#!/bin/bash
v="zxk_"`date +%Y%m%d_%H%M`
git tag ${v}
git push origin --tags
git tag --list | grep "zxk_" | tail -n3
